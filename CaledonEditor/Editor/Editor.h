/*------------------------------
| File: Editor.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "EditorContext.h"
#include "Project.h"

#include "Editor/EditorGUI.h"
#include "Windows/HierarchyPanel.h"
#include "Windows/InspectorPanel.h"
#include "Windows/ViewportPanel.h"
#include "Windows/ProjectMenu.h"

#include "CaledonEngine/DynamicLibrary.h"

namespace CE
{
	class EngineManager;
	class InputActions;
}

class Editor
{
private:
	EditorContext m_editorContext;																	// The context for the Editor, managing the selected GameObject

	EditorGUI m_editorGUI;																			// Handles GUI rendering and event management for the Editor
	HierarchyPanel m_hierarchyPanel;																// The Hierarchy panel for managing GameObjects in the current scene
	InspectorPanel m_inspectorPanel;																// The Inspector panel for viewing and editing properties of the selected GameObject
	ViewportPanel m_viewportPanel;																	// The Viewport panel for rendering the current scene
	ProjectMenu m_projectMenu;																		// The Project menu for managing project opening, creation, and module status

	Project m_project;																				// The currently loaded project
	CE::EngineManager* m_pEngineManager;															// Pointer to the EngineManager
	CE::InputActions* m_pInputActions;																// Pointer to the InputActions for the game module
	CE::DynamicLibrary m_dynamicLibrary;															// Dynamic library for loading the game module

	std::vector<std::string> m_moduleComponentTypeNames;											// List of component type names registered by the game module

	bool OpenProject(const std::string& projectFilePath);											// Opens a project from the specified file path
	bool CreateNewProject(const std::string& location, const std::string& projectName);				// Creates a new project in the specified location with the specified name
	bool FinishLoadingProject(const Project& newProject);											// Finalizes the loading of a project, including loading the game module and input actions
	void UnloadProject();																			// Unloads the currently loaded project, including the game module and input actions
	void LoadProject();																				// Loads the currently loaded project, including the game module and input actions
	void CreateEmptyScene();																		// Creates a new empty scene in the currently loaded project

public:
	Editor();																						// Constructor
	~Editor();																						// Destructor
	Editor(const Editor&) = delete;																	// Prevent copy-construction
	Editor& operator=(const Editor&) = delete;														// Prevent copy-assignment
	Editor(Editor&&) = delete;																		// Prevent move-construction
	Editor& operator=(Editor&&) = delete;															// Prevent move-assignment

	bool Initialize(const std::string& initialProjectPath);											// Prepares the Editor for use
	void Run();																						// Runs the main loop of the Editor
};