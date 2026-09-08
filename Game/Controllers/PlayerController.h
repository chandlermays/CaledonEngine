/*------------------------------
| File: PlayerController.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "CaledonEngine/Core/Component.h"

#include "Input/GameInputActions.h"

class PlayerController : public CE::Component
{
private:
	GameInputActions* m_pInputActions;											// Pointer to the Game Input Actions
	CE::InputActionMap* m_pGameplayActionMap;									// Pointer to the Gameplay action map
	
	float m_moveSpeed;															// Movement speed of the player
	float m_horizontalInput;													// Current horizontal input value
	float m_verticalInput;														// Current vertical input value

	void ConfigureInputBindings();												// Sets up input action bindings for the player

public:
	PlayerController();															// Constructor
	~PlayerController() override = default;										// Destructor
	PlayerController(const PlayerController&) = delete;							// Prevent copy-construction
	PlayerController& operator=(const PlayerController&) = delete;				// Prevent copy-assignment
	PlayerController(PlayerController&&) = delete;								// Prevent move-construction
	PlayerController& operator=(PlayerController&&) = delete;					// Prevent move-assignment

	bool Initialize() override;													// Prepares the PlayerController for use
	void Update(float deltaTime) override;										// Updates the PlayerController each frame based on input

	void SetInputActions(GameInputActions* pInputActions);						// Assigns the GameInputActions to the PlayerController

	void OnMoveHorizontal(float value);											// Handles horizontal movement input
	void OnMoveVertical(float value);											// Handles vertical movement input

	const std::string& GetTypeName() const override;							// Returns the type name of the Component
	void SetMoveSpeed(float speed);												// Sets the movement speed of the player
	float GetMoveSpeed() const;													// Returns the movement speed of the player
};