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
{
	m_pathBuffer[0] = '\0';
}

/*-------------------------------------------------------------------------
| --- SetLoadedProject: Sets the name of the currently loaded project --- |
-------------------------------------------------------------------------*/
void ProjectPanel::SetLoadedProject(const std::string& projectName)
{
	m_currentProjectName = projectName;
	m_hasProjectLoaded = true;
}

/*-------------------------------------------------------------------
| --- Draw: Draws the Project panel to load and manage projects --- |
-------------------------------------------------------------------*/
void ProjectPanel::Draw(std::function<void(const std::string&)> onOpenRequested)
{
	ImGui::Begin("Project");

	if (m_hasProjectLoaded)
	{
		ImGui::Text("Current Project: %s", m_currentProjectName.c_str());
	}
	else
	{
		ImGui::TextDisabled("No project loaded.");
	}

	ImGui::Separator();
	ImGui::InputText("Project File (.ceproj)", m_pathBuffer, sizeof(m_pathBuffer));

	if (ImGui::Button("Open Project") && m_pathBuffer[0] != '\0')
	{
		onOpenRequested(m_pathBuffer);
	}

	ImGui::End();
}