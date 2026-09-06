/*------------------------------
| File: Editor.cpp
| Author: Chandler Mays
------------------------------*/
#include "Editor.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/Manager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Systems/Tools/ToolsManager.h"
#include "CaledonEngine/Core/Scene.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------
| --- Constructor: Constructs the Editor with default values --- |
----------------------------------------------------------------*/
Editor::Editor()
	: m_pEngineManager{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
Editor::~Editor()
{
	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
	}
}

/*-------------------------------------------------
| --- Initialize: Prepares the Editor for use --- |
-------------------------------------------------*/
bool Editor::Initialize()
{
	m_pEngineManager = &CE::EngineManager::GetInstance();
	if (!m_pEngineManager->Initialize())
		return false;

	CE::ToolsManager* pToolsManager = m_pEngineManager->GetToolsManager();
	if (pToolsManager)
	{
		pToolsManager->GetDebugOverlay().SetVisible(true);
		pToolsManager->GetDebugOverlay().AddPanel([this]() { m_hierarchyPanel.Draw(); });
	}

	CreateEmptyScene();

	return true;
}

/*-----------------------------------------------
| --- Run: Runs the main loop of the Editor --- |
-----------------------------------------------*/
void Editor::Run()
{
	m_pEngineManager->Run();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------
| --- CreateEmptyScene: Creates an empty scene for the Editor to work with --- |
------------------------------------------------------------------------------*/
void Editor::CreateEmptyScene()
{
	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	auto pScene = std::make_unique<CE::Scene>();
	pScene->SetName("New Untitled");

	CE::Scene* pSceneRef = pScene.get();
	pSceneManager->AddScene(std::move(pScene));
	pSceneManager->SetCurrentScene(pSceneRef);
	pSceneRef->Initialize();
}