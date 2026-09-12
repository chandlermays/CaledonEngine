/*------------------------------
| File: GameInputActions.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "CaledonEngine/Systems/Input/InputActions.h"

class GameInputActions : public CE::InputActions
{
	// This 'GameInputActions' class acts as the instance of InputActions for the Game project.
	// Here, we can set up the action maps, input actions, and their bindings specific to the Game.
	// i.e. GameplayMap -> Move, Jump, Shoot, etc.

private:
	CE::InputActionMap* m_gameplayActionMap;										// Pointer to the Gameplay action map

	void ConfigureGameplayMap();													// Sets up the Gameplay action map and its actions

public:
	GameInputActions();																// Constructor
	~GameInputActions() = default;													// Destructor
	GameInputActions(const GameInputActions&) = delete;								// Prevent copy-construction
	GameInputActions& operator=(const GameInputActions&) = delete;					// Prevent copy-assignment
	GameInputActions(GameInputActions&&) = delete;									// Prevent move-construction
	GameInputActions& operator=(GameInputActions&&) = delete;						// Prevent move-assignment

	void Initialize();																// Prepares the Game Input Actions for use

	CE::InputActionMap* GetGameplayActionMap() const;								// Returns a pointer to the Gameplay action map
};