/*------------------------------
| File: Editor.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Windows/HierarchyPanel.h"

namespace CE
{
	class EngineManager;
}

class Editor
{
private:
	CE::EngineManager* m_pEngineManager;					// Pointer to the EngineManager
	HierarchyPanel m_hierarchyPanel;						// The Hierarchy panel for managing GameObjects in the current scene

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