#include "Game.h"
#include "CaledonEngine/Engine/EngineManager.h"
#include "CaledonEngine/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"
#include "CaledonEngine/Components/SpriteComponent.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------
| --- Constructor: 
------------------------------------------------------*/
Game::Game()
	: m_pEngineManager{ nullptr }
{}

/*--------------------------------------------------
| --- Destructor: 
--------------------------------------------------*/
Game::~Game()
{
	Shutdown();
}

/*--------------------------------------------------
| --- Initialize: 
--------------------------------------------------*/
bool Game::Initialize()
{
	m_pEngineManager = &CE::EngineManager::GetInstance();

	if (!m_pEngineManager->Initialize())
	{
		return false;
	}

	return true;
}

/*--------------------------------------------------
| --- Run:
--------------------------------------------------*/
void Game::Run()
{
	CreateScenes();

	m_pEngineManager->Run();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------
| --- CreateScenes:
------------------------------------------------------*/
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

void Game::Shutdown()
{
	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
		m_pEngineManager = nullptr;
	}
}