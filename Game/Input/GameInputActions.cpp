#include "GameInputActions.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------------
| --- Constructor: Constructs the PlayerController with default values --- |
--------------------------------------------------------------------------*/
GameInputActions::GameInputActions()
	: CE::InputActions("GameInputActions")
	, m_gameplayActionMap{ nullptr }
{}

/*-------------------------------------------------------------
| --- Initialize: Prepares the Game Input Actions for use --- |
-------------------------------------------------------------*/
void GameInputActions::Initialize()
{
	ConfigureGameplayMap();
}

/*----------------------------------------------------------------------------
| --- GetGameplayActionMap: Returns a pointer to the Gameplay action map --- |
----------------------------------------------------------------------------*/
CE::InputActionMap* GameInputActions::GetGameplayActionMap() const
{
	return m_gameplayActionMap;
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*----------------------------------------------------------------------------
| --- ConfigureGameplay: Sets up the Gameplay action map and its actions --- |
----------------------------------------------------------------------------*/
void GameInputActions::ConfigureGameplayMap()
{
	AddActionMap("Gameplay");

	// Ensure the action map was created successfully
	m_gameplayActionMap = const_cast<CE::InputActionMap*>(GetActionMapByName("Gameplay"));
	if (!m_gameplayActionMap)
		return;

	// -- Movement Input Actions --

	// Horizontal Movement
	m_gameplayActionMap->AddAction("MoveHorizontal", CE::ActionType::kValue);
	CE::InputAction* pMoveHorizontal = m_gameplayActionMap->GetActionByName("MoveHorizontal");
	if (pMoveHorizontal)
	{
		pMoveHorizontal->SetControlType(CE::ControlType::kAxis);
		pMoveHorizontal->AddPositiveBinding(CE::KeyCode::kD);
		pMoveHorizontal->AddPositiveBinding(CE::KeyCode::kRight);
		pMoveHorizontal->AddNegativeBinding(CE::KeyCode::kA);
		pMoveHorizontal->AddNegativeBinding(CE::KeyCode::kLeft);
	}

	// Vertical Movement
	m_gameplayActionMap->AddAction("MoveVertical", CE::ActionType::kValue);
	CE::InputAction* pMoveVertical = m_gameplayActionMap->GetActionByName("MoveVertical");
	if (pMoveVertical)
	{
		pMoveVertical->SetControlType(CE::ControlType::kAxis);
		pMoveVertical->AddPositiveBinding(CE::KeyCode::kW);
		pMoveVertical->AddPositiveBinding(CE::KeyCode::kUp);
		pMoveVertical->AddNegativeBinding(CE::KeyCode::kS);
		pMoveVertical->AddNegativeBinding(CE::KeyCode::kDown);
	}

	m_gameplayActionMap->Enable();
}