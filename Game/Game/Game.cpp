/*------------------------------
| File: Game.cpp
| Author: Chandler Mays
------------------------------*/
#include "Game.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Resources/ResourceManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/ComponentFactory.h"
#include "CaledonEngine/GameObjectCreator.h"

#include "Controllers/PlayerController.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the Game instance with default values --- |
-----------------------------------------------------------------------*/
Game::Game()
	: m_pEngineManager{ nullptr }
	, m_pGameObjectCreator{ nullptr }
	, m_pInputActions{ nullptr }
{}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
Game::~Game()
{
	Shutdown();
}

/*----------------------------------------------------------------
| --- Initialize: Prepares the game by booting up the engine --- |
----------------------------------------------------------------*/
bool Game::Initialize()
{
	m_pEngineManager = &CE::EngineManager::GetInstance();

	if (!m_pEngineManager->Initialize())
		return false;

	m_pGameObjectCreator = new CE::GameObjectCreator();

	m_pInputActions = new GameInputActions();
	m_pInputActions->Initialize();

	m_pEngineManager->GetInputManager()->SetInputActions(m_pInputActions);
	RegisterGameComponents();

	return true;
}

/*----------------------------------------
| --- Run: Starts the main game loop --- |
----------------------------------------*/
void Game::Run()
{
	CreateScenes();

	m_pEngineManager->Run();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-----------------------------------------------------------------------------------------------
| --- RegisterGameComponents: Registers Game-side component types with the ComponentFactory --- |
-----------------------------------------------------------------------------------------------*/
void Game::RegisterGameComponents()
{
	CE::ComponentFactory::RegisterComponent("PlayerController",
		[this](CE::GameObject*, tinyxml2::XMLElement*) -> CE::Component*
		{
			PlayerController* pController = new PlayerController();
			pController->SetInputActions(m_pInputActions);
			return pController;
		});
}

/*-----------------------------------------------------------------------------
| --- CreateScenes: Constructs and configures all game scenes and objects --- |
-----------------------------------------------------------------------------*/
void Game::CreateScenes()
{
	LoadScenes("Assets/MasterAssets.xml");
}

/*------------------------------------------------------------------------------------------
| --- LoadWorldObjects: Loads all GameObjects listed in a master XML file into a scene --- |
------------------------------------------------------------------------------------------*/
void Game::LoadScenes(const std::string& masterXmlPath)
{
	CE::ResourceManager* pResourceManager = m_pEngineManager->GetResourceManager();
	if (!pResourceManager)
		return;

	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	auto sceneFiles = pResourceManager->LoadMasterXML(masterXmlPath);

	for (const auto& [name, path] : sceneFiles)
	{
		std::string fileData = pResourceManager->GetResource(path);
		if (fileData.empty())
		{
			CE_LOG("Game::LoadScenes - Failed to load scene file '{}' ({})", name, path);
			continue;
		}

		auto pScene = std::make_unique<CE::Scene>();
		pScene->SetName(name);

		std::vector<CE::GameObject*> gameObjects = m_pGameObjectCreator->CreateGameObjects(fileData);
		for (CE::GameObject* pGameObject : gameObjects)
		{
			pScene->AddGameObject(std::unique_ptr<CE::GameObject>(pGameObject));
		}

		CE::Scene* pSceneRef = pScene.get();
		pSceneManager->AddScene(std::move(pScene));
		pSceneRef->Initialize();
	}
}

/*-----------------------------------------------------------------
| --- Shutdown: Shuts down the game and engine, and cleans up --- |
-----------------------------------------------------------------*/
void Game::Shutdown()
{
	if (m_pGameObjectCreator)
	{
		delete m_pGameObjectCreator;
		m_pGameObjectCreator = nullptr;
	}

	if (m_pInputActions)
	{
		delete m_pInputActions;
		m_pInputActions = nullptr;
	}

	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
		m_pEngineManager = nullptr;
	}
}