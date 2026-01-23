#include "Game.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"
#include "CaledonEngine/Components/SpriteComponent.h"
#include "Game/Controllers/PlayerController.h"

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
	{
		return false;
	}

	m_pInputActions = new GameInputActions();
	m_pInputActions->Initialize();

	m_pEngineManager->GetInputManager()->SetInputActions(m_pInputActions);

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
/*-----------------------------------------------------------------------------
| --- CreateScenes: Constructs and configures all game scenes and objects --- |
-----------------------------------------------------------------------------*/
void Game::CreateScenes()
{
	// Create the Scene
	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	CE::Scene* pMainScene = new CE::Scene();
	pMainScene->SetName("MainScene");

	// Create the GameObject
	CE::GameObject* pPlayer = new CE::GameObject();
	pPlayer->SetName("Player");
	pPlayer->SetTag("Player");

	pPlayer->GetTransform().SetPosition(CE::VectorFloat(100.0f, 100.0f));

	// Create & Attach the SpriteComponent
	CE::SpriteComponent* pSpriteComponent = new CE::SpriteComponent();
	pSpriteComponent->SetColor(255, 0, 0, 255);	// Red color
	pSpriteComponent->SetSize(50, 50);			// 50x50 size
	pPlayer->AddComponent(pSpriteComponent);

	// Create & Attach the PlayerController
	PlayerController* pPlayerController = new PlayerController();
	pPlayerController->SetInputActions(m_pInputActions);
	pPlayer->AddComponent(pPlayerController);

	// Add it all together!
	pMainScene->AddGameObject(pPlayer);

	pSceneManager->AddScene(pMainScene);
	pSceneManager->SetCurrentScene(pMainScene);

	pMainScene->Initialize();
}

/*-----------------------------------------------------------------
| --- Shutdown: Shuts down the game and engine, and cleans up --- |
-----------------------------------------------------------------*/
void Game::Shutdown()
{
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