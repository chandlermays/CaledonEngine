/*------------------------------
| File: Game.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "CaledonEngine/DynamicLibrary.h"

#include <string>
#include <memory>

namespace CE
{
	class EngineManager;
	class GameObjectCreator;
	class Scene;
	class InputActions;
}

class Game
{
private:
	CE::EngineManager* m_pEngineManager;												// Pointer to the Engine Manager
	CE::InputActions* m_pInputActions;													// Pointer to the Game Input Actions
	CE::DynamicLibrary m_dynamicLibrary;												// Dynamic library for loading the game module

	std::unique_ptr<CE::GameObjectCreator> m_pGameObjectCreator;						// Pointer to the Game Object Creator

	bool LoadGameModule();																// Loads the game module and retrieves the input actions and component registration functions
	void RegisterGameComponents();														// Registers Game-side component types with the ComponentFactory
	void CreateScenes();																// Constructs and configures all game scenes and objects
	void LoadScenes(const std::string& masterXmlPath);									// Loads all GameObjects listed in a master XML file into a scene

	void Shutdown();																	// Shuts down the game and engine, and cleans up

public:
	Game();																				// Constructor
	~Game();																			// Destructor
	Game(const Game&) = delete;															// Prevent copy-construction
	Game& operator=(const Game&) = delete;												// Prevent copy-assignment
	Game(Game&&) = delete;																// Prevent move-construction
	Game& operator=(Game&&) = delete;													// Prevent move-assignment

	bool Initialize();																	// Prepares the game by booting up the engine
	void Run();																			// Starts the main game loop
};