/*------------------------------
| File: ProjectMenu.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <functional>
#include <string>

class ProjectMenu
{
private:
	char m_openPathBuffer[512];																	// Buffer for the path of the project to open
	char m_newLocationBuffer[512];																// Buffer for the location of the new project to create
	char m_newNameBuffer[128];																	// Buffer for the name of the new project to create

	std::string m_currentProjectName;															// Name of the currently loaded project
	bool m_hasProjectLoaded;																	// Flag indicating whether a project is currently loaded

	std::string m_moduleStatusMessage;															// Status message for the module of the currently loaded project
	bool m_isModuleLoaded;																		// Flag indicating whether the module of the currently loaded project is loaded

	void DrawNewProjectPopup(std::function<void(const std::string&,
		const std::string&)> onCreateRequested);												// Draws the new project creation pop-up dialog, invoking the callback to create a new project

	void DrawOpenProjectPopup(std::function<void(const std::string&)> onOpenRequested);			// Draws the open project pop-up dialog, invoking the callback to open a project
	void BrowseForOpenPath();																	// Opens a native file dialog to browse for a project file path to open

public:
	ProjectMenu();																				// Constructor
	~ProjectMenu() = default;																	// Destructor
	ProjectMenu(const ProjectMenu&) = delete;													// Prevent copy-construction
	ProjectMenu& operator=(const ProjectMenu&) = delete;										// Prevent copy-assignment
	ProjectMenu(ProjectMenu&&) = delete;														// Prevent move-construction
	ProjectMenu& operator=(ProjectMenu&&) = delete;												// Prevent move-assignment

	void ClearLoadedProject();
	void SetLoadedProject(const std::string& projectName);										// Sets the name of the currently loaded project
	void SetModuleStatus(bool isLoaded, const std::string& statusMessage);						// Sets the status of the module of the currently loaded project

	void DrawMenuBar(std::function<void(const std::string&)> onOpenRequested, 
		std::function<void(const std::string&, const std::string&)> onCreateRequested);			// Draws the top main menu bar and handles user interactions for project actions
};