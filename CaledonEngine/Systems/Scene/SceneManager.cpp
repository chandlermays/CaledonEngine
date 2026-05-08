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

/*-------------------------------------------------------
| --- Initialize: Prepares the SceneManager for use --- |
-------------------------------------------------------*/
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
	m_pScenes.clear();
	m_pCurrentScene = nullptr;
}

/*-----------------------------------------------------
| --- AddScene: Add a scene to the list of scenes --- |
-----------------------------------------------------*/
void CE::SceneManager::AddScene(std::unique_ptr<Scene> pScene)
{
	if (!pScene)
		return;

	if (m_pCurrentScene == nullptr)
	{
		m_pCurrentScene = pScene.get();
	}

	m_pScenes.emplace_back(std::move(pScene));
}

/*-----------------------------------------------------------------------
| --- SetCurrentScene: Sets the provided scene as the current scene --- |
-----------------------------------------------------------------------*/
void CE::SceneManager::SetCurrentScene(Scene* pScene)
{
	if (pScene != nullptr)
	{
		m_pCurrentScene = pScene;
	}
}

/*----------------------------------------------------
| --- GetCurrentScene: Returns the current scene --- |
----------------------------------------------------*/
CE::Scene* CE::SceneManager::GetCurrentScene() const
{
	return m_pCurrentScene;
}

/*----------------------------------------------------------------
| --- GetSceneIndex: Returns the index of the provided scene --- |
----------------------------------------------------------------*/
int CE::SceneManager::GetSceneIndex(Scene* pScene) const
{
	if (!pScene)
		return -1;

	for (int i = 0; i < static_cast<int>(m_pScenes.size()); ++i)
	{
		if (m_pScenes[i].get() == pScene)
			return i;
	}

	return -1;
}