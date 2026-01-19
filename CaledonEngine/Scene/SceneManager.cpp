#include "SceneManager.h"
#include "CaledonEngine/Core/Scene.h"

/*----------------------------------------------------------------------
| --- Constructor: Initialize the SceneManager with default values --- |
----------------------------------------------------------------------*/
CE::SceneManager::SceneManager()
	: m_pCurrentScene{ nullptr }
{}

/*----------------------------------------------------------------------
| --- Destructor: Shutdown the SceneManager and clean up resources --- |
----------------------------------------------------------------------*/
CE::SceneManager::~SceneManager()
{
	Shutdown();
}

/*-------------------------------------------------
| --- Initialize: Initialize the SceneManager --- |
-------------------------------------------------*/
bool CE::SceneManager::Initialize()
{
	if (m_pCurrentScene != nullptr)
	{
		if (!m_pCurrentScene->Initialize())
		{
			return false;
		}
	}
	return true;
}

/*---------------------------------------------------------------
| --- Update: Update all scenes managed by the SceneManager --- |
---------------------------------------------------------------*/
void CE::SceneManager::Update(float deltaTime)
{
	if (m_pCurrentScene != nullptr && m_pCurrentScene->IsActive())
	{
		m_pCurrentScene->Update(deltaTime);
	}
}

/*---------------------------------------------------------------
| --- Render: Render all scenes managed by the SceneManager --- |
---------------------------------------------------------------*/
void CE::SceneManager::Render()
{
	if (m_pCurrentScene != nullptr && m_pCurrentScene->IsActive())
	{
		m_pCurrentScene->Render();
	}
}

/*--------------------------------------------------------------------
| --- Shutdown: Shutdown the SceneManager and clean up resources --- |
--------------------------------------------------------------------*/
void CE::SceneManager::Shutdown()
{
	for (Scene* pScene : m_pScenes)
	{
		delete pScene;
		pScene = nullptr;
	}
	m_pScenes.clear();
	m_pCurrentScene = nullptr;
}

/*--------------------------------------------------------------------
| --- AddScene: Add a scene to the SceneManager's list of scenes --- |
--------------------------------------------------------------------*/
void CE::SceneManager::AddScene(Scene* pScene)
{
	if (pScene != nullptr)
	{
		m_pScenes.emplace_back(pScene);

		// If there is no current scene, set the added scene as the current scene
		if (m_pCurrentScene == nullptr)
		{
			m_pCurrentScene = pScene;
		}
	}
}

/*------------------------------------------------------------------
| --- SetActiveScene: Set the current scene in the SceneManager --- |
------------------------------------------------------------------*/
void CE::SceneManager::SetCurrentScene(Scene* pScene)
{
	if (pScene != nullptr)
	{
		m_pCurrentScene = pScene;
	}
}

/*--------------------------------------------------------------------
| --- GetActiveScene: Get the current scene from the SceneManager --- |
--------------------------------------------------------------------*/
CE::Scene* CE::SceneManager::GetCurrentScene() const
{
	return m_pCurrentScene;
}

/*---------------------------------------------------------------------
| --- GetSceneIndex: Get the index of a scene in the SceneManager --- |
---------------------------------------------------------------------*/
int CE::SceneManager::GetSceneIndex(Scene* pScene) const
{
	return 0;
}