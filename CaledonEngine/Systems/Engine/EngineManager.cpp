#include "EngineManager.h"
#include "LoggingManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Systems/Tools/ToolsManager.h"
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
	for (auto* manager : m_pManagers)
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

		if (m_pInputManager->ProcessEvents())
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
		auto manager = *it;
		manager->Shutdown();
		delete manager;
		manager = nullptr;
	}

	m_pManagers.clear();

	LoggingManager::GetInstance().Shutdown();
}

/*----------------------------------------------------------------------
| --- GetGraphicsManager: Returns a pointer to the GraphicsManager --- |
----------------------------------------------------------------------*/
CE::GraphicsManager* CE::EngineManager::GetGraphicsManager() const
{
	return m_pGraphicsManager;
}

/*----------------------------------------------------------------
| --- GetInputManager: Returns a pointer to the InputManager --- |
----------------------------------------------------------------*/
CE::InputManager* CE::EngineManager::GetInputManager() const
{
	return m_pInputManager;
}

/*----------------------------------------------------------------
| --- GetSceneManager: Returns a pointer to the SceneManager --- |
----------------------------------------------------------------*/
CE::SceneManager* CE::EngineManager::GetSceneManager() const
{
	return m_pSceneManager;
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
/*-----------------------------------------------------------------------------
| --- Constructor: Constructs and registers all engine subsystem managers --- |
-----------------------------------------------------------------------------*/
CE::EngineManager::EngineManager()
	: m_isRunning{ true }
{
	m_pGraphicsManager = new GraphicsManager();
	m_pManagers.emplace_back(m_pGraphicsManager);

	m_pInputManager = new InputManager();
	m_pManagers.emplace_back(m_pInputManager);

	m_pSceneManager = new SceneManager();
	m_pManagers.emplace_back(m_pSceneManager);

	m_pToolsManager = new ToolsManager();
	m_pManagers.emplace_back(m_pToolsManager);
}

/*-------------------------------------------------------
| --- Update: Updates all engine subsystem managers --- |
-------------------------------------------------------*/
void CE::EngineManager::Update(float deltaTime)
{
	for (auto* manager : m_pManagers)
	{
		manager->Update(deltaTime);
	}
}

/*-------------------------------------------------------
| --- Render: Renders all engine subsystem managers --- |
-------------------------------------------------------*/
void CE::EngineManager::Render()
{
	for (auto* manager : m_pManagers)
	{
		manager->Render();
	}
}