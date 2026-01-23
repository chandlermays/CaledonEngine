#include "InputManager.h"
#include "CaledonEngine/Utilities/APIConfig.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------
| --- Constructor: Constructs the InputManager with default values --- |
----------------------------------------------------------------------*/
CE::InputManager::InputManager()
	: m_pInputAPI{ nullptr }
	, m_pInputActions{ nullptr }
{}

/*---------------------------------------------------
| --- Initialize: Initializes the API for Input --- |
---------------------------------------------------*/
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
| --- Shutdown: Shuts down and cleans up the InputManager --- |
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
		bool result = m_pInputAPI->ProcessEvents();
		if (!result)	// If the program is being shut down, break out now.
			return false;

		ProcessInputActions();

		return result;
	}

	return false;
}

void CE::InputManager::SetInputActions(InputActions* pInputActions)
{
	m_pInputActions = pInputActions;
}

CE::InputActions* CE::InputManager::GetInputActions() const
{
	return m_pInputActions;
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------
| --- ProcessInputActions: 
------------------------------------------------------------------*/
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
			ProcessAction(pAction);
		}
	}
}

/*------------------------------------------------------------------
| --- ProcessAction:
------------------------------------------------------------------*/
void CE::InputManager::ProcessAction(InputAction* pAction)
{

}

/*------------------------------------------------------------------
| --- CalculateAxisValue:
------------------------------------------------------------------*/
float CE::InputManager::CalculateAxisValue(InputAction* pAction)
{
	return 0.0f;
}