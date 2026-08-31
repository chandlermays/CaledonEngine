/*------------------------------
| File: Game.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Input/GameInputActions.h"

namespace CE
{
	class EngineManager;
	class GameObjectCreator;
	class Scene;
}

class Game
{
private:
	CE::EngineManager* m_pEngineManager;												// Pointer to the Engine Manager
	CE::GameObjectCreator* m_pGameObjectCreator;										// Pointer to the Game Object Creator
	GameInputActions* m_pInputActions;													// Pointer to the Game Input Actions

	void RegisterGameComponents();														// Registers Game-side component types with the ComponentFactory
	void CreateScenes();																// Constructs and configures all game scenes and objects
	void LoadScenes(const std::string& masterXmlPath);									// Loads all GameObjects listed in a master XML file into a scene

	void Shutdown();																	// Shuts down the game and engine, and cleans up

public:
	Game();																				// Constructor
	~Game();																			// Destructor
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	bool Initialize();																	// Prepares the game by booting up the engine
	void Run();																			// Starts the main game loop
};