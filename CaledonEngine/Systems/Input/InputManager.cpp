#include "InputManager.h"
#include "CaledonEngine/Utilities/APIConfig.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the Input Manager with default values --- |
-----------------------------------------------------------------------*/
CE::InputManager::InputManager()
	: m_pInputAPI{ nullptr }
	, m_pInputActions{ nullptr }
{}

/*--------------------------------------------------------
| --- Initialize: Prepares the Input Manager for use --- |
--------------------------------------------------------*/
bool CE::InputManager::Initialize()
{
	m_pInputAPI = std::make_unique<CEInput>();
	if (m_pInputAPI)
	{
		return m_pInputAPI->Initialize();
	}
	return false;
}

/*-------------------------------------------------------------
| --- Shutdown: Cleans up and shuts down the input system --- |
-------------------------------------------------------------*/
void CE::InputManager::Shutdown()
{
	if (m_pInputAPI)
	{
		m_pInputAPI->Shutdown();
		m_pInputAPI.reset();
	}

	m_pInputActions = nullptr;
}

/*------------------------------------------------------------------
| --- ProcessEvents: Processes input events from the input API --- |
------------------------------------------------------------------*/
bool CE::InputManager::ProcessEvents()
{
	if (m_pInputAPI)
	{
		bool quitRequested = m_pInputAPI->ProcessEvents();

		if (quitRequested)
			return true;

		ProcessInputActions();
	}

	return false;
}

/*-------------------------------------------------------------------------
| --- SetInputActions: Assigns the Input Actions to the Input Manager --- |
-------------------------------------------------------------------------*/
void CE::InputManager::SetInputActions(InputActions* pInputActions)
{
	m_pInputActions = pInputActions;
}

/*-----------------------------------------------------------
| --- GetInputAPI: Returns a pointer to the Input's API --- |
-----------------------------------------------------------*/
CE::Input* CE::InputManager::GetInputAPI() const
{
	return m_pInputAPI.get();
}

/*-------------------------------------------------------------
| --- GetInputActions: Returns the assigned Input Actions --- |
-------------------------------------------------------------*/
CE::InputActions* CE::InputManager::GetInputActions() const
{
	return m_pInputActions;
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*----------------------------------------------------------
| --- ProcessInputActions: Processes all input actions --- |
----------------------------------------------------------*/
void CE::InputManager::ProcessInputActions()
{
	if (!m_pInputActions || !m_pInputActions->IsEnabled())
		return;

	const auto& actionMaps = m_pInputActions->GetInputActionMaps();

	for (const auto& [mapName, pActionMap] : actionMaps)
	{
		if (!pActionMap->IsEnabled())
			continue;

		const auto& actions = pActionMap->GetInputActions();
		for (const auto& [actionName, pAction] : actions)
		{
			ProcessAction(pAction.get());
		}
	}
}

/*--------------------------------------------------------
| --- ProcessAction: Processes a single input action --- |
--------------------------------------------------------*/
void CE::InputManager::ProcessAction(InputAction* pAction)
{
	if (!pAction || !pAction->IsEnabled())
		return;

	ActionType type = pAction->GetActionType();

	if (type == ActionType::kValue)
	{
		float value = CalculateAxisValue(pAction);
		pAction->InvokeValueCallbacks(value);
	}
	else if (type == ActionType::kButton)
	{
		bool isPressed = false;

		const auto& bindings = pAction->GetInputBindings();
		for (const auto& binding : bindings)
		{
			if (binding.m_isMouseButton)
			{
				if (m_pInputAPI->IsMouseButtonPressed(binding.m_mouseCode))
				{
					isPressed = true;
					break;
				}
			}
			else
			{
				if (m_pInputAPI->IsKeyPressed(binding.m_keyCode))
				{
					isPressed = true;
					break;
				}
			}
		}

		if (isPressed)
		{
			pAction->InvokeStartedCallbacks();
			pAction->InvokePerformedCallbacks();
		}

		// Additional logic for released state should be added here for Canceled callbacks
	}
}

/*-------------------------------------------------------------------------------------
| --- CalculateAxisValue: Calculates the axis value for a Value type input action --- |
-------------------------------------------------------------------------------------*/
float CE::InputManager::CalculateAxisValue(InputAction* pAction)
{
	if (!pAction || !m_pInputAPI)
		return 0.0f;

	float value = 0.0f;

	const auto& compositeBindings = pAction->GetCompositeBindings();

	if (!compositeBindings.empty())
	{
		const CompositeBinding& composite = compositeBindings[0];

		// Check positive bindings
		for (const auto& binding : composite.m_positiveBindings)
		{
			if (binding.m_isMouseButton)
			{
				if (m_pInputAPI->IsMouseButtonHeld(binding.m_mouseCode))
				{
					value += 1.0f;
					break;
				}
			}
			else
			{
				if (m_pInputAPI->IsKeyHeld(binding.m_keyCode))
				{
					value += 1.0f;
					break;
				}
			}
		}

		// Check negative bindings
		for (const auto& binding : composite.m_negativeBindings)
		{
			if (binding.m_isMouseButton)
			{
				if (m_pInputAPI->IsMouseButtonHeld(binding.m_mouseCode))
				{
					value -= 1.0f;
					break;
				}
			}
			else
			{
				if (m_pInputAPI->IsKeyHeld(binding.m_keyCode))
				{
					value -= 1.0f;
					break;
				}
			}
		}
	}
	else
	{
		// Otherwise, check regular bindings
		const auto& bindings = pAction->GetInputBindings();
		for (const auto& binding : bindings)
		{
			if (binding.m_isMouseButton)
			{
				if (m_pInputAPI->IsMouseButtonHeld(binding.m_mouseCode))
				{
					value += 1.0f;
					break;
				}
			}
			else
			{
				if (m_pInputAPI->IsKeyHeld(binding.m_keyCode))
				{
					value += 1.0f;
					break;
				}
			}
		}
	}

	// Clamp the value to [-1, 1] range
	if (value > 1.0f) value = 1.0f;
	if (value < -1.0f) value = -1.0f;

	return value;
}