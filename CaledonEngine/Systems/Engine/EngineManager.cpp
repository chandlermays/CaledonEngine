/*------------------------------
| File: EngineManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "EngineManager.h"
#include "LoggingManager.h"

#include "Systems/Physics/CollisionManager.h"
#include "Systems/Rendering/GraphicsManager.h"
#include "Systems/Resources/ResourceManager.h"
#include "Systems/Input/InputManager.h"
#include "Systems/Scene/SceneManager.h"
#include "Systems/Tools/ToolsManager.h"

#include <chrono>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::EngineManager::~EngineManager()
{
	CE_LOG("EngineManager::~EngineManager - Shutting down EngineManager.");
	Shutdown();
}

/*--------------------------------------------------------------------------
| --- GetInstance: Returns the singleton instance of the EngineManager --- |
--------------------------------------------------------------------------*/
CE::EngineManager& CE::EngineManager::GetInstance()
{
	static EngineManager instance;
	return instance;
}

/*---------------------------------------------------------------
| --- Initialize: Initializes all engine subsystem managers --- |
---------------------------------------------------------------*/
bool CE::EngineManager::Initialize()
{
	if (!LoggingManager::GetInstance().Initialize())
	{
		return false;
	}

	bool success = true;
	for (const auto& manager : m_pManagers)
	{
		if (!manager->Initialize())
		{
			success = false;
			break;
		}
	}

	return success;
}

/*-------------------------------------------------------------------------------
| --- Run: Main loop that updates and renders all engine subsystem managers --- |
-------------------------------------------------------------------------------*/
void CE::EngineManager::Run()
{
	auto lastFrameTime = std::chrono::high_resolution_clock::now();

	while (m_isRunning)
	{
		auto thisFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> frameDuration = thisFrameTime - lastFrameTime;
		float deltaTime = frameDuration.count();
		lastFrameTime = thisFrameTime;

		bool quitRequested = m_pInputManager->ProcessEvents();

		if (quitRequested)
		{
			m_isRunning = false;
			break;
		}

		Update(deltaTime);
		Render();
	}
}

/*--------------------------------------------------------------------------
| --- Shutdown: Shuts down and cleans up all engine subsystem managers --- |
--------------------------------------------------------------------------*/
void CE::EngineManager::Shutdown()
{
	if (m_pManagers.empty())
		return;

	for (auto it = m_pManagers.rbegin(); it != m_pManagers.rend(); ++it)
	{
		(*it)->Shutdown();
	}

	m_pManagers.clear();
	m_pUpdatables.clear();
	m_pRenderables.clear();

	// Null out observers
	m_pGraphicsManager = nullptr;
	m_pResourceManager = nullptr;
	m_pSceneManager = nullptr;
	m_pCollisionManager = nullptr;
	m_pInputManager = nullptr;
	m_pToolsManager = nullptr;

	LoggingManager::GetInstance().Shutdown();
}

/*----------------------------------------------------------------------
| --- GetGraphicsManager: Returns a pointer to the GraphicsManager --- |
----------------------------------------------------------------------*/
CE::GraphicsManager* CE::EngineManager::GetGraphicsManager() const
{
	return m_pGraphicsManager;
}

/*----------------------------------------------------------------------
| --- GetResourceManager: Returns a pointer to the ResourceManager --- |
----------------------------------------------------------------------*/
CE::ResourceManager* CE::EngineManager::GetResourceManager() const
{
	return m_pResourceManager;
}

/*----------------------------------------------------------------
| --- GetSceneManager: Returns a pointer to the SceneManager --- |
----------------------------------------------------------------*/
CE::SceneManager* CE::EngineManager::GetSceneManager() const
{
	return m_pSceneManager;
}

/*------------------------------------------------------------------------
| --- GetCollisionManager: Returns a pointer to the CollisionManager --- |
------------------------------------------------------------------------*/
CE::CollisionManager* CE::EngineManager::GetCollisionManager() const
{
	return m_pCollisionManager;
}

/*----------------------------------------------------------------
| --- GetInputManager: Returns a pointer to the InputManager --- |
----------------------------------------------------------------*/
CE::InputManager* CE::EngineManager::GetInputManager() const
{
	return m_pInputManager;
}

/*----------------------------------------------------------------
| --- GetToolsManager: Returns a pointer to the ToolsManager --- |
----------------------------------------------------------------*/
CE::ToolsManager* CE::EngineManager::GetToolsManager() const
{
	return m_pToolsManager;
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the EngineManager with default values --- |
-----------------------------------------------------------------------*/
CE::EngineManager::EngineManager()
	: m_isRunning{ true }
	, m_pGraphicsManager{ nullptr }
	, m_pResourceManager{ nullptr }
	, m_pSceneManager{ nullptr }
	, m_pCollisionManager{ nullptr }
	, m_pInputManager{ nullptr }
	, m_pToolsManager{ nullptr }
{
	auto pGraphics = std::make_unique<GraphicsManager>();
	m_pGraphicsManager = pGraphics.get();
	RegisterManager(std::move(pGraphics));

	auto pResource = std::make_unique<ResourceManager>();
	m_pResourceManager = pResource.get();
	RegisterManager(std::move(pResource));

	auto pScene = std::make_unique<SceneManager>();
	m_pSceneManager = pScene.get();
	RegisterManager(std::move(pScene));

	auto pCollision = std::make_unique<CollisionManager>();
	m_pCollisionManager = pCollision.get();
	RegisterManager(std::move(pCollision));

	auto pInput = std::make_unique<InputManager>();
	m_pInputManager = pInput.get();
	RegisterManager(std::move(pInput));

	auto pTools = std::make_unique<ToolsManager>();
	m_pToolsManager = pTools.get();
	RegisterManager(std::move(pTools));
}

/*--------------------------------------------------------------
| --- RegisterManager: Registers a manager with the engine --- |
--------------------------------------------------------------*/
void CE::EngineManager::RegisterManager(std::unique_ptr<CE::Manager> pManager)
{
	if (auto* pUpdatable = dynamic_cast<IUpdatable*>(pManager.get()))
	{
		m_pUpdatables.emplace_back(pUpdatable);
	}

	if (auto* pRenderable = dynamic_cast<IRenderable*>(pManager.get()))
	{
		m_pRenderables.emplace_back(pRenderable);
	}

	m_pManagers.emplace_back(std::move(pManager));
}

/*-------------------------------------------------------
| --- Update: Updates all engine subsystem managers --- |
-------------------------------------------------------*/
void CE::EngineManager::Update(float deltaTime)
{
	for (IUpdatable* pUpdatable : m_pUpdatables)
	{
		pUpdatable->Update(deltaTime);
	}
}

/*-------------------------------------------------------
| --- Render: Renders all engine subsystem managers --- |
-------------------------------------------------------*/
void CE::EngineManager::Render()
{
	if (m_pGraphicsManager)
	{
		m_pGraphicsManager->BeginFrame();
	}

	for (IRenderable* pRenderable : m_pRenderables)
	{
		pRenderable->Render();
	}

	if (m_pGraphicsManager)
	{
		m_pGraphicsManager->EndFrame();
	}
}