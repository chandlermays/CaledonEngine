#pragma once
#include "CaledonEngine/Systems/Input/InputActions.h"

class GameInputActions : public CE::InputActions
{
	// This 'GameInputActions' class acts as the instance of InputActions for the Game project.
	// Here, we can set up the action maps, input actions, and their bindings specific to the Game.
	// i.e. GameplayMap -> Move, Jump, Shoot, etc.

private:
	CE::InputActionMap* m_gameplayActionMap;

	void ConfigureGameplayMap();

public:
	GameInputActions();
	~GameInputActions() = default;
	GameInputActions(const GameInputActions&) = delete;
	GameInputActions& operator=(const GameInputActions&) = delete;
	GameInputActions(GameInputActions&&) = delete;
	GameInputActions& operator=(GameInputActions&&) = delete;

	void Initialize();

	CE::InputActionMap* GetGameplayActionMap() const;
};