/*------------------------------
| File: HierarchyPanel.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <functional>
#include <string>

class ProjectPanel
{
private:
	char m_openPathBuffer[512];																	// Buffer for the path of the project to open
	char m_newRootBuffer[512];																	// Buffer for the root directory of the new project to create
	char m_newNameBuffer[128];																	// Buffer for the name of the new project to create

	std::string m_currentProjectName;															// Name of the currently loaded project
	bool m_hasProjectLoaded;																	// Flag indicating whether a project is currently loaded

	std::string m_moduleStatusMessage;															// Status message for the module of the currently loaded project
	bool m_isModuleLoaded;																		// Flag indicating whether the module of the currently loaded project is loaded

public:
	ProjectPanel();																				// Constructor
	~ProjectPanel() = default;																	// Destructor
	ProjectPanel(const ProjectPanel&) = delete;													// Prevent copy-construction
	ProjectPanel& operator=(const ProjectPanel&) = delete;										// Prevent copy-assignment
	ProjectPanel(ProjectPanel&&) = delete;														// Prevent move-construction
	ProjectPanel& operator=(ProjectPanel&&) = delete;											// Prevent move-assignment

	void SetLoadedProject(const std::string& projectName);										// Sets the name of the currently loaded project
	void SetModuleStatus(bool isLoaded, const std::string& statusMessage);						// Sets the status of the module of the currently loaded project

	void Draw(std::function<void(const std::string&)> onOpenRequested,							// Draws the project panel and handles user interactions
		std::function<void(const std::string&, const std::string&)> onCreateRequested);
};