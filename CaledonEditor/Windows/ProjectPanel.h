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
	char m_pathBuffer[512];														// Buffer for the project file path input
	std::string m_currentProjectName;											// Name of the currently loaded project
	bool m_hasProjectLoaded;													// Flag indicating whether a project is currently loaded

public:
	ProjectPanel();																// Constructor
	~ProjectPanel() = default;													// Destructor
	ProjectPanel(const ProjectPanel&) = delete;									// Prevent copy-construction
	ProjectPanel& operator=(const ProjectPanel&) = delete;						// Prevent copy-assignment
	ProjectPanel(ProjectPanel&&) = delete;										// Prevent move-construction
	ProjectPanel& operator=(ProjectPanel&&) = delete;							// Prevent move-assignment

	void SetLoadedProject(const std::string& projectName);						// Sets the name of the currently loaded project
	void Draw(std::function<void(const std::string&)> onOpenRequested);			// Draws the Project panel to load and manage projects
};