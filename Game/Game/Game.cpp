#include "Game.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Resources/ResourceManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/ComponentFactory.h"
#include "CaledonEngine/GameObjectCreator.h"
#include "CaledonEngine/DynamicLibraryInterface.h"

Game::Game()
	: m_pEngineManager{ nullptr }
	, m_pGameObjectCreator{ nullptr }
	, m_pInputActions{ nullptr }
{}

Game::~Game() { Shutdown(); }

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

void Game::Run()
{
	CreateScenes();
	m_pEngineManager->Run();
}

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

void Game::RegisterGameComponents()
{
	auto registerComponents = reinterpret_cast<CE::DynamicLibraryInterface::RegisterComponentsFunc>(
		m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kRegisterComponentsFunctionName));
	if (registerComponents)
		registerComponents(&CE::ComponentFactory::RegisterComponent);
	else
		CE_LOG("Game::RegisterGameComponents - Module is missing '{}'", CE::DynamicLibraryInterface::kRegisterComponentsFunctionName);
}

void Game::CreateScenes() { LoadScenes("Assets/MasterAssets.xml"); }

void Game::LoadScenes(const std::string& masterXmlPath)
{
	CE::ResourceManager* pResourceManager = m_pEngineManager->GetResourceManager();
	if (!pResourceManager || !m_pGameObjectCreator)
		return;

	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();
	for (const auto& [name, path] : pResourceManager->LoadMasterXML(masterXmlPath))
	{
		std::string fileData = pResourceManager->GetResource(path);
		if (fileData.empty())
		{
			CE_LOG("Game::LoadScenes - Failed to load scene file '{}' ({})", name, path);
			continue;
		}

		auto pScene = std::make_unique<CE::Scene>();
		pScene->SetName(name);
		for (auto& pGameObject : m_pGameObjectCreator->CreateGameObjects(fileData))
			pScene->AddGameObject(std::move(pGameObject));

		CE::Scene* pSceneRef = pScene.get();
		pSceneManager->AddScene(std::move(pScene));
		pSceneRef->Initialize();
	}
}

void Game::Shutdown()
{
	if (m_pEngineManager)
	{
		// Destroy scene-owned module components while the module is still loaded.
		m_pEngineManager->Shutdown();
		m_pEngineManager = nullptr;
	}

	m_pGameObjectCreator.reset();
	delete m_pInputActions;
	m_pInputActions = nullptr;
	m_dynamicLibrary.Unload();
}
