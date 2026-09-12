/*------------------------------
| File: Editor.cpp
| Author: Chandler Mays
------------------------------*/
#include "Editor.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Systems/Tools/ToolsManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/ComponentFactory.h"
#include "CaledonEngine/DynamicLibraryInterface.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------
| --- Constructor: Constructs the Editor with default values --- |
----------------------------------------------------------------*/
Editor::Editor()
	: m_pEngineManager{ nullptr }
	, m_pInputActions{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
Editor::~Editor()
{
	if (m_pInputActions)
	{
		delete m_pInputActions;
		m_pInputActions = nullptr;
	}

	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
	}

	m_dynamicLibrary.Unload();
}

/*-------------------------------------------------
| --- Initialize: Prepares the Editor for use --- |
-------------------------------------------------*/
bool Editor::Initialize()
{
	m_pEngineManager = &CE::EngineManager::GetInstance();
	if (!m_pEngineManager->Initialize())
		return false;

	if (!LoadGameModule())
	{
		CE_LOG("Editor::Initialize - No game module loaded; only Engine components will be available.");
	}
	else
	{
		m_pEngineManager->GetInputManager()->SetInputActions(m_pInputActions);
		RegisterGameComponents();
	}

	CE::ToolsManager* pToolsManager = m_pEngineManager->GetToolsManager();
	if (pToolsManager)
	{
		pToolsManager->GetDebugOverlay().SetVisible(true);
		pToolsManager->GetDebugOverlay().AddPanel([this]() { m_hierarchyPanel.Draw(m_editorContext); });
		pToolsManager->GetDebugOverlay().AddPanel([this]() { m_inspectorPanel.Draw(m_editorContext); });
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
/*--------------------------------------------------------------------------------------------------------------------
| --- LoadGameModule: Loads the game module and retrieves the input actions and component registration functions --- |
--------------------------------------------------------------------------------------------------------------------*/
bool Editor::LoadGameModule()
{
	if (!m_dynamicLibrary.Load("PacManModule.dll"))		// TODO: This should not be hard-coded in the Editor
		return false;

	auto createInputActions = reinterpret_cast<CE::DynamicLibraryInterface::CreateInputActionsFunc>(
		m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kCreateInputActionsFunctionName));

	if (!createInputActions)
		return false;

	m_pInputActions = createInputActions();
	return m_pInputActions != nullptr;
}

/*-----------------------------------------------------------------------------------------------
| --- RegisterGameComponents: Registers Game-side component types with the ComponentFactory --- |
-----------------------------------------------------------------------------------------------*/
void Editor::RegisterGameComponents()
{
	auto registerComponents = reinterpret_cast<CE::DynamicLibraryInterface::RegisterComponentsFunc>(
		m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kRegisterComponentsFunctionName));

	if (registerComponents)
	{
		registerComponents(&CE::ComponentFactory::RegisterComponent);
	}
}

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