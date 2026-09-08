/*------------------------------
| File: Editor.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "EditorContext.h"

#include "Windows/HierarchyPanel.h"
#include "Windows/InspectorPanel.h"

namespace CE
{
	class EngineManager;
}

class Editor
{
private:
	CE::EngineManager* m_pEngineManager;					// Pointer to the EngineManager
	EditorContext m_editorContext;							// The context for the Editor, managing the selected GameObject
	HierarchyPanel m_hierarchyPanel;						// The Hierarchy panel for managing GameObjects in the current scene
	InspectorPanel m_inspectorPanel;						// The Inspector panel for viewing and editing properties of the selected GameObject

	void CreateEmptyScene();								// Creates an empty scene for the Editor to work with

public:
	Editor();												// Constructor
	~Editor();												// Destructor
	Editor(const Editor&) = delete;							// Prevent copy-construction
	Editor& operator=(const Editor&) = delete;				// Prevent copy-assignment
	Editor(Editor&&) = delete;								// Prevent move-construction
	Editor& operator=(Editor&&) = delete;					// Prevent move-assignment

	bool Initialize();										// Prepares the Editor for use
	void Run();												// Runs the main loop of the Editor
};