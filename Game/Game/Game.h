#pragma once
#include "CaledonEngine/DynamicLibrary.h"
#include <memory>
#include <string>

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
	CE::EngineManager* m_pEngineManager;
	std::unique_ptr<CE::GameObjectCreator> m_pGameObjectCreator;
	CE::InputActions* m_pInputActions;
	CE::DynamicLibrary m_dynamicLibrary;

	bool LoadGameModule();
	void RegisterGameComponents();
	void CreateScenes();
	void LoadScenes(const std::string& masterXmlPath);
	void Shutdown();

public:
	Game();
	~Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	bool Initialize();
	void Run();
};
