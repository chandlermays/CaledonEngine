#include "HierarchyPanel.h"
#include "Systems/Engine/EngineManager.h"
#include "Systems/Scene/SceneManager.h"
#include "Core/Scene.h"
#include "Core/GameObject.h"

#include <ImGUI/imgui.h>

HierarchyPanel::HierarchyPanel() : m_createdCount{ 0 } {}

void HierarchyPanel::Draw()
{
	CE::SceneManager* pSceneManager = CE::EngineManager::GetInstance().GetSceneManager();
	CE::Scene* pScene = pSceneManager ? pSceneManager->GetCurrentScene() : nullptr;

	ImGui::Begin("Hierarchy");

	if (!pScene)
	{
		ImGui::Text("No active scene.");
		ImGui::End();
		return;
	}

	if (ImGui::Button("Create GameObject"))
	{
		auto pNewObject = std::make_unique<CE::GameObject>();
		pNewObject->SetName("GameObject (" + std::to_string(m_createdCount++) + ")");
		CE::GameObject* pRef = pNewObject.get();
		pScene->AddGameObject(std::move(pNewObject));
		pRef->Initialize();
	}

	ImGui::Separator();

	for (const auto& pGameObject : pScene->GetGameObjects())
	{
		DrawGameObjectNode(pGameObject.get());
	}

	ImGui::End();
}

void HierarchyPanel::DrawGameObjectNode(CE::GameObject* pGameObject)
{
	if (!pGameObject)
		return;

	const auto& children = pGameObject->GetChildren();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool isOpen = ImGui::TreeNodeEx(pGameObject->GetName().c_str(), flags);

	if (isOpen && !children.empty())
	{
		for (CE::GameObject* pChild : children)
		{
			DrawGameObjectNode(pChild);
		}
		ImGui::TreePop();
	}
}