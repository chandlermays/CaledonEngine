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
#include "CaledonEngine/DynamicLibraryInterface.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the Game instance with default values --- |
-----------------------------------------------------------------------*/
Game::Game()
	: m_pEngineManager{ nullptr }
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

	m_pGameObjectCreator = std::make_unique<CE::GameObjectCreator>();

	if (!LoadGameModule())
	{
		CE_LOG("Game::Initialize - Failed to load .dll");
		return false;
	}

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
/*--------------------------------------------------------------------------------------------------------------------
| --- LoadGameModule: Loads the game module and retrieves the input actions and component registration functions --- |
--------------------------------------------------------------------------------------------------------------------*/
bool Game::LoadGameModule()
{
	if (!m_dynamicLibrary.Load("PacManModule.dll"))
	{
		CE_LOG("Game::LoadGameModule - Could not load .dll");
		return false;
	}

	auto createInputActions = reinterpret_cast<CE::DynamicLibraryInterface::CreateInputActionsFunc>(
		m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kCreateInputActionsFunctionName));

	if (!createInputActions)
	{
		CE_LOG("Game::LoadGameModule - Module is missing '{}'", CE::DynamicLibraryInterface::kCreateInputActionsFunctionName);
		return false;
	}

	m_pInputActions = createInputActions();
	return m_pInputActions != nullptr;
}

/*-----------------------------------------------------------------------------------------------
| --- RegisterGameComponents: Registers Game-side component types with the ComponentFactory --- |
-----------------------------------------------------------------------------------------------*/
void Game::RegisterGameComponents()
{
	auto registerComponents = reinterpret_cast<CE::DynamicLibraryInterface::RegisterComponentsFunc>(
		m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kRegisterComponentsFunctionName));

	if (registerComponents)
	{
		registerComponents(&CE::ComponentFactory::RegisterComponent);
	}
	else
	{
		CE_LOG("Game::RegisterGameComponents - Module is missing '{}'", CE::DynamicLibraryInterface::kRegisterComponentsFunctionName);
	}
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

		std::vector<std::unique_ptr<CE::GameObject>> gameObjects = m_pGameObjectCreator->CreateGameObjects(fileData);
		for (auto& pGameObject : gameObjects)
		{
			pScene->AddGameObject(std::move(pGameObject));
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
	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
		m_pEngineManager = nullptr;
	}

	if (m_pInputActions)
	{
		auto destroyInputActions =
			reinterpret_cast<CE::DynamicLibraryInterface::DestroyInputActionsFunc>(
				m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kDestroyInputActionsFunctionName));

		if (destroyInputActions)
		{
			destroyInputActions(m_pInputActions);
		}
		else
		{
			CE_LOG("Game::Shutdown - Could not find 'DestroyModuleInputActions' in module.");
		}

		m_pInputActions = nullptr;
	}

	if (m_pGameObjectCreator)
	{
		m_pGameObjectCreator.reset();
	}

	CE::ComponentFactory::Clear();

	m_dynamicLibrary.Unload();
}