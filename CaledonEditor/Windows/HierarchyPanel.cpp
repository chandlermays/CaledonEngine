/*------------------------------
| File: HierarchyPanel.cpp
| Author: Chandler Mays
------------------------------*/
#include "HierarchyPanel.h"
#include "Editor/EditorContext.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Core/GameObject.h"

#include <CaledonEngine/lib/ImGUI/imgui.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------
| --- Constructor: Constructs the HierarchyPanel with default values --- |
------------------------------------------------------------------------*/
HierarchyPanel::HierarchyPanel()
	: m_createdCount{ 0 }
{ }

/*----------------------------------------------------------------------------------------------
| --- Draw: Draws the Hierarchy panel to create and store GameObjects in the current scene --- |
----------------------------------------------------------------------------------------------*/
void HierarchyPanel::Draw(EditorContext& context)
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
		pNewObject->SetName("GameObject (" + std::to_string(++m_createdCount) + ")");
		CE::GameObject* pRef = pNewObject.get();
		pScene->AddGameObject(std::move(pNewObject));
		pRef->Initialize();
		context.SetSelected(pRef);
	}

	ImGui::Separator();

	for (const auto& pGameObject : pScene->GetGameObjects())
	{
		DrawGameObjectNode(pGameObject.get(), context);
	}

	ImGui::End();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-----------------------------------------------------------------------------
| --- DrawGameObjectNode: Draws a single GameObject node in the hierarchy --- |
-----------------------------------------------------------------------------*/
void HierarchyPanel::DrawGameObjectNode(CE::GameObject* pGameObject, EditorContext& context)
{
	if (!pGameObject)
		return;

	const auto& children = pGameObject->GetChildren();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	if (pGameObject == context.GetSelected())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool isOpen = ImGui::TreeNodeEx(pGameObject->GetName().c_str(), flags);

	if (ImGui::IsItemClicked())
	{
		context.SetSelected(pGameObject);
	}

	if (isOpen && !children.empty())
	{
		for (CE::GameObject* pChild : children)
		{
			DrawGameObjectNode(pChild, context);
		}
		ImGui::TreePop();
	}
}