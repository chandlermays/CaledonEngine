#include "Game.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"
#include "CaledonEngine/Components/SpriteComponent.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the Game instance with default values --- |
-----------------------------------------------------------------------*/
Game::Game()
	: m_pEngineManager{ nullptr }
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
	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	CE::Scene* pMainScene = new CE::Scene();
	pMainScene->SetName("MainScene");

	CE::GameObject* pPlayer = new CE::GameObject();
	pPlayer->SetName("Player");
	pPlayer->SetTag("Player");

	pPlayer->GetTransform().SetPosition(CE::VectorFloat(100.0f, 100.0f));

	CE::SpriteComponent* pSpriteComponent = new CE::SpriteComponent();
	pSpriteComponent->SetColor(255, 0, 0, 255);	// Red color
	pSpriteComponent->SetSize(50, 50);			// 50x50 size
	pPlayer->AddComponent(pSpriteComponent);

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
	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
		m_pEngineManager = nullptr;
	}
}