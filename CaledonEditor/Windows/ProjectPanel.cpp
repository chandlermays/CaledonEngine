/*------------------------------
| File: ProjectPanel.cpp
| Author: Chandler Mays
------------------------------*/
#include "ProjectPanel.h"

#include <CaledonEngine/lib/ImGUI/imgui.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------
| --- Constructor: Constructs the ProjectPanel with default values --- |
----------------------------------------------------------------------*/
ProjectPanel::ProjectPanel()
	: m_hasProjectLoaded{ false }
	, m_isModuleLoaded{ false }
{
	m_openPathBuffer[0] = '\0';
	m_newRootBuffer[0] = '\0';
	m_newNameBuffer[0] = '\0';
}

/*-------------------------------------------------------------------------
| --- SetLoadedProject: Sets the name of the currently loaded project --- |
-------------------------------------------------------------------------*/
void ProjectPanel::SetLoadedProject(const std::string& projectName)
{
	m_currentProjectName = projectName;
	m_hasProjectLoaded = true;
}

/*----------------------------------------------------------------------------------------
| --- SetModuleStatus: Sets the status of the module of the currently loaded project --- |
----------------------------------------------------------------------------------------*/
void ProjectPanel::SetModuleStatus(bool isLoaded, const std::string & statusMessage)
{
	m_isModuleLoaded = isLoaded;
	m_moduleStatusMessage = statusMessage;
}

/*---------------------------------------------------------------------
| --- Draw: Draws the project panel and handles user interactions --- |
---------------------------------------------------------------------*/
void ProjectPanel::Draw(std::function<void(const std::string&)> onOpenRequested, std::function<void(const std::string&, const std::string&)> onCreateRequested)
{
	ImGui::Begin("Project");

	if (m_hasProjectLoaded)
	{
		ImGui::Text("Current Project: %s", m_currentProjectName.c_str());
		if (m_isModuleLoaded)
		{
			ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Module loaded.");
		}
		else
		{
			ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "%s", m_moduleStatusMessage.c_str());
			ImGui::TextDisabled("(Running with Engine components only.)");
		}
	}
	else
	{
		ImGui::TextDisabled("No project loaded.");
	}

	ImGui::Separator();

	// Prototype stand-in for File > Open Project with a native dialog.
	ImGui::Text("Open Existing Project");
	ImGui::InputText("Project File (.ceproj)##open", m_openPathBuffer, sizeof(m_openPathBuffer));
	if (ImGui::Button("Open Project") && m_openPathBuffer[0] != '\0')
	{
		onOpenRequested(m_openPathBuffer);
	}

	ImGui::Separator();

	// Prototype stand-in for File > New Project.
	ImGui::Text("Create New Project");
	ImGui::InputText("Root Directory##new", m_newRootBuffer, sizeof(m_newRootBuffer));
	ImGui::InputText("Project Name##new", m_newNameBuffer, sizeof(m_newNameBuffer));
	if (ImGui::Button("Create Project") && m_newRootBuffer[0] != '\0' && m_newNameBuffer[0] != '\0')
	{
		onCreateRequested(m_newRootBuffer, m_newNameBuffer);
	}

	ImGui::End();
}