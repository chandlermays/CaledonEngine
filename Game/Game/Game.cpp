/*------------------------------
| File: Game.cpp
| Author: Chandler Mays
------------------------------*/
#include "Game.h"
#include "../Controllers/PlayerController.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Resources/ResourceManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/ComponentFactory.h"
#include "CaledonEngine/GameObjectCreator.h"

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
	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	auto pMainScene = std::make_unique<CE::Scene>();
	pMainScene->SetName("MainScene");

	// Hand-built for now, until the Player is authored as XML content too
	auto pPlayer = std::make_unique<CE::GameObject>();
	pPlayer->SetName("Player");
	pPlayer->SetTag("Player");

	PlayerController* pPlayerController = new PlayerController();
	pPlayerController->SetInputActions(m_pInputActions);
	pPlayer->AddComponent(pPlayerController);
	pMainScene->AddGameObject(std::move(pPlayer));

	LoadWorldObjects(pMainScene.get(), "Assets/MasterAssets.xml");

	CE::Scene* pSceneRef = pMainScene.get();
	pSceneManager->AddScene(std::move(pMainScene));
	pSceneManager->SetCurrentScene(pSceneRef);
	pSceneRef->Initialize();
}

/*------------------------------------------------------------------------------------------
| --- LoadWorldObjects: Loads all GameObjects listed in a master XML file into a scene --- |
------------------------------------------------------------------------------------------*/
void Game::LoadWorldObjects(CE::Scene* pScene, const std::string& masterXmlPath)
{
	if (!pScene)
		return;

	CE::ResourceManager* pResourceManager = m_pEngineManager->GetResourceManager();
	if (!pResourceManager)
		return;

	auto subsystemFiles = pResourceManager->LoadMasterXML(masterXmlPath);

	for (const auto& [name, path] : subsystemFiles)
	{
		std::string fileData = pResourceManager->GetResource(path);
		if (fileData.empty())
		{
			CE_LOG("Game::LoadWorldObjects - Failed to load subsystem file '{}' ({})", name, path);
			continue;
		}

		std::vector<CE::GameObject*> gameObjects = m_pGameObjectCreator->CreateGameObjects(fileData);
		for (CE::GameObject* pGameObject : gameObjects)
		{
			pScene->AddGameObject(std::unique_ptr<CE::GameObject>(pGameObject));
		}
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