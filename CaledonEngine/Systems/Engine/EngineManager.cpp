/*------------------------------
| File: EngineManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "EngineManager.h"
#include "LoggingManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Resources/ResourceManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
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

	// Null out observers
	m_pGraphicsManager = nullptr;
	m_pResourceManager = nullptr;
	m_pSceneManager = nullptr;
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
/*-----------------------------------------------------------------------------
| --- Constructor: Constructs and registers all engine subsystem managers --- |
-----------------------------------------------------------------------------*/
CE::EngineManager::EngineManager()
	: m_isRunning{ true }
	, m_pGraphicsManager{ nullptr }
	, m_pResourceManager{ nullptr }
	, m_pSceneManager{ nullptr }
	, m_pInputManager{ nullptr }
	, m_pToolsManager{ nullptr }
{
	auto pGraphics = std::make_unique<GraphicsManager>();
	m_pGraphicsManager = pGraphics.get();
	m_pManagers.emplace_back(std::move(pGraphics));

	auto pResource = std::make_unique<ResourceManager>();
	m_pResourceManager = pResource.get();
	m_pManagers.emplace_back(std::move(pResource));

	auto pScene = std::make_unique<SceneManager>();
	m_pSceneManager = pScene.get();
	m_pManagers.emplace_back(std::move(pScene));

	auto pInput = std::make_unique<InputManager>();
	m_pInputManager = pInput.get();
	m_pManagers.emplace_back(std::move(pInput));

	auto pTools = std::make_unique<ToolsManager>();
	m_pToolsManager = pTools.get();
	m_pManagers.emplace_back(std::move(pTools));
}

/*-------------------------------------------------------
| --- Update: Updates all engine subsystem managers --- |
-------------------------------------------------------*/
void CE::EngineManager::Update(float deltaTime)
{
	for (const auto& manager : m_pManagers)
	{
		manager->Update(deltaTime);
	}
}

/*-------------------------------------------------------
| --- Render: Renders all engine subsystem managers --- |
-------------------------------------------------------*/
void CE::EngineManager::Render()
{
	for (const auto& manager : m_pManagers)
	{
		manager->Render();
	}
}