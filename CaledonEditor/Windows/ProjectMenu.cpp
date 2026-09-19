/*------------------------------
| File: ProjectMenu.cpp
| Author: Chandler Mays
------------------------------*/
#include "ProjectMenu.h"

#include <ImGUI/imgui.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------
| --- Constructor: Constructs the ProjectPanel with default values --- |
----------------------------------------------------------------------*/
ProjectMenu::ProjectMenu()
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
void ProjectMenu::SetLoadedProject(const std::string& projectName)
{
	m_currentProjectName = projectName;
	m_hasProjectLoaded = true;
}

/*----------------------------------------------------------------------------------------
| --- SetModuleStatus: Sets the status of the module of the currently loaded project --- |
----------------------------------------------------------------------------------------*/
void ProjectMenu::SetModuleStatus(bool isLoaded, const std::string & statusMessage)
{
	m_isModuleLoaded = isLoaded;
	m_moduleStatusMessage = statusMessage;
}

/*----------------------------------------------------------------------------------------
| --- DrawMenuBar:
----------------------------------------------------------------------------------------*/
void ProjectMenu::DrawMenuBar(std::function<void(const std::string&)> onOpenRequested,
	std::function<void(const std::string&, const std::string&)> onCreateRequested)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project..."))
			{
				m_newRootBuffer[0] = '\0';
				m_newNameBuffer[0] = '\0';
				ImGui::OpenPopup("New Project");
			}

			if (ImGui::MenuItem("Open Project..."))
			{
				m_openPathBuffer[0] = '\0';
				ImGui::OpenPopup("Open Project");
			}

			ImGui::EndMenu();
		}

		std::string statusText = m_hasProjectLoaded
			? ("Project: " + m_currentProjectName + (m_isModuleLoaded ? "" : "  (module not loaded)"))
			: "No project loaded";

		float textWidth = ImGui::CalcTextSize(statusText.c_str()).x;
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textWidth - 16.0f);

		if (m_hasProjectLoaded && !m_isModuleLoaded)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "%s", statusText.c_str());
		}
		else
		{
			ImGui::TextDisabled("%s", statusText.c_str());
		}

		ImGui::EndMainMenuBar();
	}

	DrawNewProjectPopup(onCreateRequested);
	DrawOpenProjectPopup(onOpenRequested);
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*----------------------------------------------------------------------------------------
| --- DrawNewProjectPopup:
----------------------------------------------------------------------------------------*/
void ProjectMenu::DrawNewProjectPopup(std::function<void(const std::string&, const std::string&)> onCreateRequested)
{
	if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Root Directory", m_newRootBuffer, sizeof(m_newRootBuffer));
		ImGui::InputText("Project Name", m_newNameBuffer, sizeof(m_newNameBuffer));

		ImGui::Separator();

		bool canCreate = m_newRootBuffer[0] != '\0' && m_newNameBuffer[0] != '\0';
		ImGui::BeginDisabled(!canCreate);
		if (ImGui::Button("Create"))
		{
			onCreateRequested(m_newRootBuffer, m_newNameBuffer);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

/*----------------------------------------------------------------------------------------
| --- DrawOpenProjectPopup:
----------------------------------------------------------------------------------------*/
void ProjectMenu::DrawOpenProjectPopup(std::function<void(const std::string&)> onOpenRequested)
{
	if (ImGui::BeginPopupModal("Open Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Project File (.ceproj)", m_openPathBuffer, sizeof(m_openPathBuffer));

		ImGui::Separator();

		bool canOpen = m_openPathBuffer[0] != '\0';
		ImGui::BeginDisabled(!canOpen);
		if (ImGui::Button("Open"))
		{
			onOpenRequested(m_openPathBuffer);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}