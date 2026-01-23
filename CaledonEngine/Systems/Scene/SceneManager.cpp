#include "SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include <CaledonEngine/Systems/Engine/LoggingManager.h>

/*----------------------------------------------------------------------
| --- Constructor: Constructs the SceneManager with default values --- |
----------------------------------------------------------------------*/
CE::SceneManager::SceneManager()
	: m_pCurrentScene{ nullptr }
{}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::SceneManager::~SceneManager()
{
	CE_LOG("SceneManager::~SceneManager - Shutting down SceneManager.");
	Shutdown();
}

/*---------------------------------------------------------------
| --- Initialize: Initialize the current scene if it exists --- |
---------------------------------------------------------------*/
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

/*------------------------------------------
| --- Update: Update the current scene --- |
------------------------------------------*/
void CE::SceneManager::Update(float deltaTime)
{
	if (m_pCurrentScene != nullptr && m_pCurrentScene->IsActive())
	{
		m_pCurrentScene->Update(deltaTime);
	}
}

/*------------------------------------------
| --- Render: Render the current scene --- |
------------------------------------------*/
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

/*-----------------------------------------------------
| --- AddScene: Add a scene to the list of scenes --- |
-----------------------------------------------------*/
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

/*----------------------------------------------------------------------
| --- SetActiveScene: Sets the provided scene as the current scene --- |
----------------------------------------------------------------------*/
void CE::SceneManager::SetCurrentScene(Scene* pScene)
{
	if (pScene != nullptr)
	{
		m_pCurrentScene = pScene;
	}
}

/*----------------------------------------------------------
| --- GetActiveScene: Returns the current active scene --- |
----------------------------------------------------------*/
CE::Scene* CE::SceneManager::GetCurrentScene() const
{
	return m_pCurrentScene;
}

/*----------------------------------------------------------------
| --- GetSceneIndex: Returns the index of the provided scene --- |
----------------------------------------------------------------*/
int CE::SceneManager::GetSceneIndex(Scene* pScene) const
{
	for (int i = 0; i < m_pScenes.size(); ++i)
	{
		if (m_pScenes[i] == pScene)
		{
			return static_cast<int>(i);
		}
	}
	return -1; // Scene not found
}