#include "Game.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"
#include "CaledonEngine/Systems/Rendering/SpriteComponent.h"
#include "CaledonEngine/Systems/Rendering/Shape.h"
#include "CaledonEngine/Systems/Rendering/Sprite.h"
#include "CaledonEngine/Systems/Rendering/Square.h"
#include "CaledonEngine/Systems/Rendering/Circle.h"
#include "CaledonEngine/Systems/Rendering/Triangle.h"
#include "CaledonEngine/Systems/Rendering/Capsule.h"
#include "CaledonEngine/Systems/Rendering/Color.h"
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

	// Create a Square
	CE::GameObject* pSquare = new CE::GameObject();
	pSquare->GetTransform().SetPosition(CE::VectorFloat(100.0f, 100.0f));
	CE::SpriteComponent* pSpriteCmp1 = new CE::SpriteComponent();
	auto pSquareShape = std::make_unique<CE::Square>(CE::Color::Red(), 100, true);
	auto pSprite1 = CE::Sprite::CreateFromShape(std::move(pSquareShape), 100);
	pSpriteCmp1->SetSprite(std::move(pSprite1));
	pSquare->AddComponent(pSpriteCmp1);

	// Create a Circle
	CE::GameObject* pCircle = new CE::GameObject();
	pCircle->GetTransform().SetPosition(CE::VectorFloat(600.0f, 100.0f));
	CE::SpriteComponent* pSpriteCmp2 = new CE::SpriteComponent();
	auto pCircleShape = std::make_unique<CE::Circle>(CE::Color::Green(), 50, true, 32);
	auto pSprite2 = CE::Sprite::CreateFromShape(std::move(pCircleShape), 100);
	pSpriteCmp2->SetSprite(std::move(pSprite2));
	pCircle->AddComponent(pSpriteCmp2);

	// Create a Triangle
	CE::GameObject* pTriangle = new CE::GameObject();
	pTriangle->GetTransform().SetPosition(CE::VectorFloat(100.0f, 600.0f));
	CE::SpriteComponent* pSpriteCmp3 = new CE::SpriteComponent();
	auto pTriangleShape = std::make_unique<CE::Triangle>(CE::Color::Magenta(), 100, 100, true);
	auto pSprite3 = CE::Sprite::CreateFromShape(std::move(pTriangleShape), 100);
	pSpriteCmp3->SetSprite(std::move(pSprite3));
	pTriangle->AddComponent(pSpriteCmp3);

	// Create a Capsule
	CE::GameObject* pCapsule = new CE::GameObject();
	pCapsule->GetTransform().SetPosition(CE::VectorFloat(600.0f, 600.0f));
	CE::SpriteComponent* pSpriteCmp4 = new CE::SpriteComponent();
	auto pCapsuleShape = std::make_unique<CE::Capsule>(CE::Color::Yellow(), 100, 200, true, 16);
	auto pSprite4 = CE::Sprite::CreateFromShape(std::move(pCapsuleShape), 100);
	pSpriteCmp4->SetSprite(std::move(pSprite4));
	pCapsule->AddComponent(pSpriteCmp4);

	// Create & Attach the PlayerController
	PlayerController* pPlayerController = new PlayerController();
	pPlayerController->SetInputActions(m_pInputActions);
	pSquare->AddComponent(pPlayerController);

	// Add it all together!
	pMainScene->AddGameObject(pSquare);
	pMainScene->AddGameObject(pCircle);
	pMainScene->AddGameObject(pTriangle);
	pMainScene->AddGameObject(pCapsule);

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