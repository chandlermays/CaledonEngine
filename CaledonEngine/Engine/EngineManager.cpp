#include "EngineManager.h"
#include "LoggingManager.h"
#include "CaledonEngine/Rendering/GraphicsManager.h"
#include "CaledonEngine/Input/InputManager.h"
#include "CaledonEngine/Physics/CollisionManager.h"
#include "CaledonEngine/Scene/SceneManager.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------------
| --- Constructor: Instantiates and registers all engine subsystem managers --- |
-------------------------------------------------------------------------------*/
CE::EngineManager::EngineManager()
	: m_isRunning{ true }
{ 
	m_pGraphicsManager = new GraphicsManager();
	m_pManagers.emplace_back(m_pGraphicsManager);

	m_pInputManager = new InputManager();
	m_pManagers.emplace_back(m_pInputManager);

	m_pCollisionManager = new CollisionManager();
	m_pManagers.emplace_back(m_pCollisionManager);

	m_pSceneManager = new SceneManager();
	m_pManagers.emplace_back(m_pSceneManager);
}

/*-----------------------------------------------------------------------
| --- GetInstance: Gets the singleton instance of the EngineManager --- |
-----------------------------------------------------------------------*/
CE::EngineManager& CE::EngineManager::GetInstance()
{
	static EngineManager instance;
	return instance;
}

/*----------------------------------------------------------------------------
| --- Destructor: Cleans up and shuts down all engine subsystem managers --- |
----------------------------------------------------------------------------*/
CE::EngineManager::~EngineManager()
{
	Shutdown();
}

/*---------------------------------------------------------------
| --- Initialize: Initializes all engine subsystem managers --- |
---------------------------------------------------------------*/
bool CE::EngineManager::Initialize()
{
	if (!LoggingManager::GetInstance().Initialize())
	{
		CE_LOG("EngineManager::Initialize - Failed to initialize LoggingManager.");
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
	float deltaTime = 0.0f;

	while (m_isRunning)
	{
		Update(deltaTime);
		Render();
	}
}

/*--------------------------------------------------------------------------
| --- Shutdown: Shuts down and cleans up all engine subsystem managers --- |
--------------------------------------------------------------------------*/
void CE::EngineManager::Shutdown()
{
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

/*----------------------------------------------------------------
| --- GetGraphicsManager: Gets access to the GraphicsManager --- |
----------------------------------------------------------------*/
CE::GraphicsManager* CE::EngineManager::GetGraphicsManager() const
{
	return m_pGraphicsManager;
}

/*----------------------------------------------------------
| --- GetInputManager: Gets access to the InputManager --- |
----------------------------------------------------------*/
CE::InputManager* CE::EngineManager::GetInputManager() const
{
	return m_pInputManager;
}

/*------------------------------------------------------------------
| --- GetCollisionManager: Gets access to the CollisionManager --- |
------------------------------------------------------------------*/
CE::CollisionManager* CE::EngineManager::GetCollisionManager() const
{
	return m_pCollisionManager;
}

/*----------------------------------------------------------
| --- GetSceneManager: Gets access to the SceneManager --- |
----------------------------------------------------------*/
CE::SceneManager* CE::EngineManager::GetSceneManager() const
{
	return m_pSceneManager;
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
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