#pragma once

namespace CE
{
	class EngineManager;
}

class Game
{
private:
	CE::EngineManager* m_pEngineManager;							// Pointer to the engine manager

	void CreateScenes();											// Create all game scenes

	void Shutdown();												// Shutdown the game

public:
	Game();															// Constructor
	~Game();														// Destructor
	Game(const Game&) = delete;										// Prevent copy-construction
	Game& operator=(const Game&) = delete;							// Prevent copy-assignment
	Game(Game&&) = delete;											// Prevent move-construction
	Game& operator=(Game&&) = delete;								// Prevent move-assignment

	bool Initialize();												// Initialize the game
	void Run();														// Run the game loop
};