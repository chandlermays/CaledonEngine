/*------------------------------
| File: Editor.cpp
| Author: Chandler Mays
------------------------------*/
#include "Editor.h"

#include "EditorSettings.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include "CaledonEngine/Systems/Resources/ResourceManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Systems/Tools/ToolsManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/GameObjectCreator.h"
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
	UnloadProject();

	m_viewportPanel.Shutdown();
	m_editorGUI.Shutdown();

	if (m_pEngineManager)
	{
		m_pEngineManager->Shutdown();
	}
}

/*-------------------------------------------------
| --- Initialize: Prepares the Editor for use --- |
-------------------------------------------------*/
bool Editor::Initialize(const std::string& initialProjectPath)
{
	Project::CaptureLaunchDirectory();

	m_pEngineManager = &CE::EngineManager::GetInstance();
	if (!m_pEngineManager->Initialize())
		return false;

	if (!m_editorGUI.Initialize())
		return false;

	CE::Input* pInputAPI = m_pEngineManager->GetInputManager()->GetInputAPI();
	if (pInputAPI)
	{
		pInputAPI->SetEventCallback([this](const void* pEvent)
			{
				m_editorGUI.ProcessEvent(pEvent);
			});
	}

	m_pEngineManager->SetFrameCallback([this]()
		{
			m_editorGUI.BeginFrame();

			m_projectMenu.DrawMenuBar(
				[this](const std::string& path) { OpenProject(path); },
				[this](const std::string& root, const std::string& name) { CreateNewProject(root, name); });

			m_editorGUI.BeginDockspace();

			m_hierarchyPanel.Draw(m_editorContext);
			m_inspectorPanel.Draw(m_editorContext);
			m_viewportPanel.Draw(m_editorContext);

			m_editorGUI.EndDockspace();
			m_editorGUI.EndFrame();
		});

	std::string effectiveProjectPath = initialProjectPath.empty()
		? EditorSettings::GetLastProjectPath()
		: initialProjectPath;

	if (!effectiveProjectPath.empty())
	{
		OpenProject(effectiveProjectPath);
	}
	else
	{
		CreateEmptyScene();
	}

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
/*-------------------------------------------------------------------
| --- OpenProject: Opens a project from the specified file path --- |
-------------------------------------------------------------------*/
bool Editor::OpenProject(const std::string& projectFilePath)
{
	UnloadProject();

	Project newProject;
	if (!newProject.Load(projectFilePath))
	{
		CE_LOG("Editor::OpenProject - Failed to load project file '{}'", projectFilePath);
		CreateEmptyScene();
		return false;
	}

	return FinishLoadingProject(newProject);
}

/*---------------------------------------------------------------------------------------------------
| --- CreateNewProject: Creates a new project in the specified location with the specified name --- |
---------------------------------------------------------------------------------------------------*/
bool Editor::CreateNewProject(const std::string& location, const std::string& projectName)
{
	UnloadProject();

	Project newProject;
	if (!newProject.CreateNew(location, projectName))
	{
		CE_LOG("Editor::CreateNewProject - Failed to create project '{}' at '{}'", projectName, location);
		CreateEmptyScene();
		return false;
	}

	return FinishLoadingProject(newProject);
}

/*-----------------------------------------------------------------------------------------------------------------------
| --- FinishLoadingProject: Finalizes the loading of a project, including loading the game module and input actions --- |
-----------------------------------------------------------------------------------------------------------------------*/
bool Editor::FinishLoadingProject(const Project& newProject)
{
	m_project = newProject;

	if (!m_dynamicLibrary.Load(m_project.GetModulePath()))
	{
		CE_LOG("Editor::FinishLoadingProject - Failed to load module '{}'; continuing with Engine components only.", m_project.GetModulePath());
		m_projectMenu.SetModuleStatus(false, "Module not found: " + m_project.GetModulePath());
	}
	else
	{
		auto createInputActions = reinterpret_cast<CE::DynamicLibraryInterface::CreateInputActionsFunc>(
			m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kCreateInputActionsFunctionName));

		if (createInputActions)
		{
			m_pInputActions = createInputActions();
			m_pEngineManager->GetInputManager()->SetInputActions(m_pInputActions);
		}

		auto registerComponents = reinterpret_cast<CE::DynamicLibraryInterface::RegisterComponentsFunc>(
			m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kRegisterComponentsFunctionName));

		if (registerComponents)
		{
			CE::ComponentFactory::RegisterFunc recordingRegister =
				[this](const std::string& typeName, const std::string& category,
					CE::ComponentFactory::XmlCreatorFunc xmlCreator, CE::ComponentFactory::DefaultCreatorFunc defaultCreator,
					std::vector<CE::PropertyDescriptor> properties, bool allowMultiple)
				{
					m_moduleComponentTypeNames.push_back(typeName);
					CE::ComponentFactory::RegisterComponent(typeName, category, std::move(xmlCreator), std::move(defaultCreator), std::move(properties), allowMultiple);
				};

			registerComponents(recordingRegister);
		}

		m_projectMenu.SetModuleStatus(true, "");
	}

	LoadProject();
	m_projectMenu.SetLoadedProject(m_project.GetName());
	EditorSettings::SetLastProjectPath(m_project.GetProjectPath());

	return true;
}

/*----------------------------------------------------------------------------------------------------------
| --- UnloadProject: Unloads the currently loaded project, including the game module and input actions --- |
----------------------------------------------------------------------------------------------------------*/
void Editor::UnloadProject()
{
	// 1. Unload active scenes and clear context selection before unloading the DLL module
	if (m_pEngineManager && m_pEngineManager->GetSceneManager())
	{
		m_pEngineManager->GetSceneManager()->UnloadAllScenes();
	}

	m_editorContext.ClearSelection();

	// Clear cached resources so relative file paths (e.g. Assets/MasterAssets.xml) reload for the next project
	if (m_pEngineManager && m_pEngineManager->GetResourceManager())
	{
		m_pEngineManager->GetResourceManager()->ClearCache();
	}

	// 2. Detach and safely destroy InputActions through the DLL interface
	if (m_pInputActions)
	{
		if (m_pEngineManager && m_pEngineManager->GetInputManager())
		{
			m_pEngineManager->GetInputManager()->SetInputActions(nullptr);
		}

		auto destroyInputActions = reinterpret_cast<CE::DynamicLibraryInterface::DestroyInputActionsFunc>(
			m_dynamicLibrary.GetFunctionAddress(CE::DynamicLibraryInterface::kDestroyInputActionsFunctionName));

		if (destroyInputActions)
		{
			destroyInputActions(m_pInputActions);
		}
		else
		{
			// Fallback cleanup if DLL lookup fails
			delete m_pInputActions;
		}

		m_pInputActions = nullptr;
	}

	// 3. Unregister custom component types from ComponentFactory
	for (const std::string& typeName : m_moduleComponentTypeNames)
	{
		CE::ComponentFactory::UnregisterComponent(typeName);
	}
	m_moduleComponentTypeNames.clear();

	// 4. Unload the dynamic library module from process memory
	m_dynamicLibrary.Unload();

	// 5. Reset project state
	m_project = Project();
	m_projectMenu.ClearLoadedProject();
}

/*------------------------------------------------------------------------------------------------------
| --- LoadProject: Loads the currently loaded project, including the game module and input actions --- |
------------------------------------------------------------------------------------------------------*/
void Editor::LoadProject()
{
	CE::ResourceManager* pResourceManager = m_pEngineManager->GetResourceManager();
	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	if (!pResourceManager || !pSceneManager || m_project.GetMasterAssetsPath().empty())
	{
		CreateEmptyScene();
		return;
	}

	auto sceneFiles = pResourceManager->LoadMasterXML(m_project.GetMasterAssetsPath());
	if (sceneFiles.empty())
	{
		CreateEmptyScene();
		return;
	}

	CE::GameObjectCreator gameObjectCreator;

	for (const auto& [name, path] : sceneFiles)
	{
		std::string fileData = pResourceManager->GetResource(path);
		if (fileData.empty())
		{
			CE_LOG("Editor::LoadProjectScenes - Failed to load scene file '{}' ({})", name, path);
			continue;
		}

		auto pScene = std::make_unique<CE::Scene>();
		pScene->SetName(name);

		std::vector<std::unique_ptr<CE::GameObject>> gameObjects = gameObjectCreator.CreateGameObjects(fileData);
		for (auto& pGameObject : gameObjects)
		{
			pScene->AddGameObject(std::move(pGameObject));
		}

		CE::Scene* pSceneRef = pScene.get();
		pSceneManager->AddScene(std::move(pScene));
		pSceneManager->SetCurrentScene(pSceneRef);
		pSceneRef->Initialize();
	}
}

/*-------------------------------------------------------------------------------------
| --- CreateEmptyScene: Creates a new empty scene in the currently loaded project --- |
-------------------------------------------------------------------------------------*/
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