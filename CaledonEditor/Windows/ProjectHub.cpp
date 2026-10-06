#include "ProjectHub.h"

#include "ProjectMenu.h"
#include "Editor/EditorSettings.h"

#include <ImGUI/imgui.h>
#include <filesystem>

void ProjectHub::Draw(ProjectMenu& projectMenu, const std::function<void(const std::string&)>& onOpenRequested)
{
	ImGuiViewport* pViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(pViewport->WorkPos);
	ImGui::SetNextWindowSize(pViewport->WorkSize);

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

	if (ImGui::Begin("ProjectHub", nullptr, flags))
	{
		ImGui::Text("CaledonEditor");
		ImGui::Separator();

		if (ImGui::Button("New Project...")) { projectMenu.RequestNewProjectPopup(); }
		ImGui::SameLine();
		if (ImGui::Button("Open Project...")) { projectMenu.RequestOpenProjectPopup(); }

		if (!m_errorMessage.empty())
		{
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", m_errorMessage.c_str());
		}

		ImGui::Separator();
		ImGui::TextDisabled("Recent Projects");

		// TODO: Cache the recent projects instead of invoking every frame
		const std::vector<std::string> recents = EditorSettings::GetRecentProjects();
		for (size_t i = 0; i < recents.size(); ++i)
		{
			ImGui::PushID(static_cast<int>(i));

			const std::string name = std::filesystem::path(recents[i]).stem().string();
			if (ImGui::Selectable(name.c_str()))
			{
				onOpenRequested(recents[i]);
			}
			ImGui::TextDisabled("%s", recents[i].c_str());

			ImGui::PopID();
		}
	}
	ImGui::End();
}

void ProjectHub::SetError(const std::string& message)
{
	m_errorMessage = message;
}

void ProjectHub::ClearError()
{
	m_errorMessage.clear();
}