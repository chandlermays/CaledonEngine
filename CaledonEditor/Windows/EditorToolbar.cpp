/*------------------------------
| File: EditorToolbar.cpp
| Author: Chandler Mays
------------------------------*/
#include "EditorToolbar.h"

#include "Editor/Editor.h"
#include "SceneSaver.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"

#include <ImGUI/imgui.h>

#include <filesystem>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------------
| --- Draw:
------------------------------------------------------------------------------*/
void EditorToolbar::Draw(Editor& editor)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

	if (ImGui::BeginMainMenuBar())
	{
		DrawPlayControls(editor);
		ImGui::Separator();
		DrawSaveControls(editor);
		ImGui::Separator();
		DrawSceneStatus(editor);

		ImGui::EndMainMenuBar();
	}
}

bool EditorToolbar::IsPlaying() const
{
	return m_isPlaying;
}

bool EditorToolbar::IsPaused() const
{
	return m_isPaused;
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------
| --- DrawPlayControls:
------------------------------------------------------------------------------*/
void EditorToolbar::DrawPlayControls(Editor& editor)
{
	// For now, these are placeholders. 
	// Full implementation would integrate with EngineManager pause state.

	if (ImGui::Button("Play", ImVec2(50, 0)))
	{
		m_isPlaying = true;
		CE_LOG("EditorToolbar: Play pressed");
	}

	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(60, 0)))
	{
		m_isPaused = !m_isPaused;
		CE_LOG("EditorToolbar: Pause pressed");
	}

	ImGui::SameLine();
	if (ImGui::Button("Stop", ImVec2(50, 0)))
	{
		m_isPlaying = false;
		m_isPaused = false;
		CE_LOG("EditorToolbar: Stop pressed");
	}

	if (ImGui::Button("New Scene", ImVec2(80, 0)))
	{
		CE::SceneManager* pSceneManager = CE::EngineManager::GetInstance().GetSceneManager();
		if (pSceneManager)
		{
			auto pScene = std::make_unique<CE::Scene>();
			pScene->SetName("Untitled Scene");
			// Don't set a file path — force user to "Save As"

			CE::Scene* pSceneRef = pScene.get();
			pSceneManager->AddScene(std::move(pScene));
			pSceneManager->SetCurrentScene(pSceneRef);
			pSceneRef->Initialize();
			pSceneRef->SetDirty(true);

			CE_LOG("EditorToolbar: New scene created");
		}
	}
}

/*------------------------------------------------------------------------------
| --- DrawSaveControls:
------------------------------------------------------------------------------*/
void EditorToolbar::DrawSaveControls(Editor& editor)
{
	CE::SceneManager* pSceneManager = CE::EngineManager::GetInstance().GetSceneManager();
	CE::Scene* pCurrentScene = pSceneManager ? pSceneManager->GetCurrentScene() : nullptr;

	if (ImGui::Button("Save Scene", ImVec2(90, 0)))
	{
		if (pCurrentScene)
		{
			// If scene has no file path, it's a new unsaved scene → open dialog
			if (pCurrentScene->GetFilePath().empty())
			{
				ImGui::OpenPopup("Save Scene As");
			}
			else
			{
				// Scene was loaded or previously saved → save to same path
				if (SceneSaver::SaveScene(*pCurrentScene))
				{
					CE_LOG("EditorToolbar: Scene '{}' saved successfully", pCurrentScene->GetName());
				}
				else
				{
					CE_LOG("EditorToolbar: Failed to save scene '{}'", pCurrentScene->GetName());
				}
			}
		}
		else
		{
			CE_LOG("EditorToolbar: No active scene to save");
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Save All", ImVec2(70, 0)))
	{
		// TODO: Iterate all scenes and save each
	}

	ImGui::SameLine();
	if (ImGui::Button("Save As...", ImVec2(80, 0)))
	{
		ImGui::OpenPopup("Save Scene As");
	}

	DrawSaveAsDialog(editor);
}

/*------------------------------------------------------------------------------
| --- DrawSaveAsDialog:
------------------------------------------------------------------------------*/
void EditorToolbar::DrawSaveAsDialog(Editor& editor)
{
	CE::SceneManager* pSceneManager = CE::EngineManager::GetInstance().GetSceneManager();
	CE::Scene* pCurrentScene = pSceneManager ? pSceneManager->GetCurrentScene() : nullptr;

	if (!pCurrentScene)
		return;

	static char fileNameBuffer[256] = {};

	if (ImGui::BeginPopupModal("Save Scene As", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Scene Name:");
		ImGui::SameLine();
		ImGui::InputText("##sceneName", fileNameBuffer, sizeof(fileNameBuffer));

		ImGui::Text("Location: Assets/Scenes/");

		ImGui::Separator();

		bool canSave = strlen(fileNameBuffer) > 0;
		ImGui::BeginDisabled(!canSave);

		if (ImGui::Button("Save", ImVec2(120, 0)))
		{
			std::string fileName = fileNameBuffer;

			// Ensure .xml extension
			if (fileName.find(".xml") == std::string::npos)
			{
				fileName += ".xml";
			}

			// Build full path: Assets/Scenes/<FileName>.xml
			std::string fullPath = "Assets/Scenes/" + fileName;

			if (SceneSaver::SaveSceneAs(*pCurrentScene, fullPath))
			{
				const std::string sceneName = std::filesystem::path(fileName).stem().string();

				pCurrentScene->SetName(sceneName);
				editor.GetProject().RegisterScene(sceneName, fullPath);

				CE_LOG("EditorToolbar: Scene saved as '{}'", fullPath);
				fileNameBuffer[0] = '\0';
				ImGui::CloseCurrentPopup();
			}
			else
			{
				CE_LOG("EditorToolbar: Failed to save scene as '{}'", fullPath);
			}
		}

		ImGui::EndDisabled();

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			fileNameBuffer[0] = '\0';  // Clear buffer
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

/*------------------------------------------------------------------------------
| --- DrawSceneStatus:
------------------------------------------------------------------------------*/
void EditorToolbar::DrawSceneStatus(Editor& editor)
{
	CE::SceneManager* pSceneManager = CE::EngineManager::GetInstance().GetSceneManager();
	CE::Scene* pCurrentScene = pSceneManager ? pSceneManager->GetCurrentScene() : nullptr;

	if (!pCurrentScene)
	{
		ImGui::TextDisabled("No scene loaded");
		return;
	}

	std::string statusText = "Scene: " + pCurrentScene->GetName();
	if (pCurrentScene->IsDirty())
	{
		// Display red dot and unsaved indicator
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "\u25CF");  // Red circle
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%s (unsaved)", statusText.c_str());
	}
	else
	{
		// Display green dot and saved indicator
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "\u25CF");  // Green circle
		ImGui::SameLine();
		ImGui::Text("%s", statusText.c_str());
	}

	ImGui::SameLine();
	if (pCurrentScene->GetFilePath().empty())
	{
		ImGui::TextDisabled("(not yet saved)");
	}
	else
	{
		ImGui::TextDisabled("%s", pCurrentScene->GetFilePath().c_str());
	}
}