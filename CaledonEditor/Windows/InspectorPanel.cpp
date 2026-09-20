/*------------------------------
| File: InspectorPanel.cpp
| Author: Chandler Mays
------------------------------*/
#include "InspectorPanel.h"
#include "Editor/EditorContext.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Component.h"
#include "CaledonEngine/ComponentFactory.h"
#include "CaledonEngine/ComponentTypeInfo.h"
#include <ImGUI/imgui.h>
#include <unordered_map>
#include <vector>

void InspectorPanel::Draw(EditorContext& context)
{
	ImGui::Begin("Inspector");
	CE::GameObject* pGameObject = context.GetSelected();
	if (!pGameObject)
	{
		ImGui::TextDisabled("No GameObject selected.");
		ImGui::End();
		return;
	}

	static char nameBuffer[256];
	strncpy_s(nameBuffer, pGameObject->GetName().c_str(), sizeof(nameBuffer) - 1);
	if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
		pGameObject->SetName(nameBuffer);

	ImGui::Separator();
	for (const auto& pComponent : pGameObject->GetAllComponents())
		DrawComponent(pComponent.get());
	ImGui::Separator();
	DrawAddComponentMenu(pGameObject);
	ImGui::End();
}

void InspectorPanel::DrawComponent(CE::Component* pComponent)
{
	if (!pComponent) return;
	const std::string& typeName = pComponent->GetTypeName();
	const auto* pTypeInfo = CE::ComponentFactory::GetTypeInfo(typeName);
	ImGui::PushID(pComponent);
	if (ImGui::CollapsingHeader(typeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!pTypeInfo || pTypeInfo->properties.empty())
			ImGui::TextDisabled("(no editable properties)");
		else
			for (const auto& property : pTypeInfo->properties)
				DrawProperty(pComponent, property);
	}
	ImGui::PopID();
}

void InspectorPanel::DrawProperty(CE::Component* pComponent, const CE::PropertyDescriptor& property)
{
	CE::PropertyValue value = property.getter(pComponent);
	if (std::holds_alternative<float>(value))
	{
		float v = std::get<float>(value);
		if (ImGui::DragFloat(property.name.c_str(), &v)) property.setter(pComponent, v);
	}
	else if (std::holds_alternative<int>(value))
	{
		int v = std::get<int>(value);
		if (ImGui::DragInt(property.name.c_str(), &v)) property.setter(pComponent, v);
	}
	else if (std::holds_alternative<bool>(value))
	{
		bool v = std::get<bool>(value);
		if (ImGui::Checkbox(property.name.c_str(), &v)) property.setter(pComponent, v);
	}
	else if (std::holds_alternative<CE::Vector2f>(value))
	{
		CE::Vector2f v = std::get<CE::Vector2f>(value);
		float components[2] = { v.x, v.y };
		if (ImGui::DragFloat2(property.name.c_str(), components)) property.setter(pComponent, CE::Vector2f(components[0], components[1]));
	}
	else if (std::holds_alternative<CE::Color>(value))
	{
		CE::Color v = std::get<CE::Color>(value);
		float components[4] = { v.r / 255.0f, v.g / 255.0f, v.b / 255.0f, v.a / 255.0f };
		if (ImGui::ColorEdit4(property.name.c_str(), components))
			property.setter(pComponent, CE::Color(static_cast<std::uint8_t>(components[0] * 255.0f), static_cast<std::uint8_t>(components[1] * 255.0f), static_cast<std::uint8_t>(components[2] * 255.0f), static_cast<std::uint8_t>(components[3] * 255.0f)));
	}
	else if (std::holds_alternative<std::string>(value))
	{
		std::string v = std::get<std::string>(value);
		char buffer[256];
		strncpy_s(buffer, v.c_str(), sizeof(buffer) - 1);
		if (ImGui::InputText(property.name.c_str(), buffer, sizeof(buffer))) property.setter(pComponent, std::string(buffer));
	}
}

void InspectorPanel::DrawAddComponentMenu(CE::GameObject* pGameObject)
{
	if (ImGui::Button("Add Component")) ImGui::OpenPopup("AddComponentPopup");
	if (!ImGui::BeginPopup("AddComponentPopup")) return;

	std::unordered_map<std::string, std::vector<std::string>> categorized;
	for (const auto& [typeName, entry] : CE::ComponentFactory::GetAllRegisteredTypes())
		if (entry.defaultCreator) categorized[entry.category].push_back(typeName);

	for (const auto& [category, typeNames] : categorized)
	{
		ImGui::Separator();
		ImGui::Text("%s", category.c_str());
		ImGui::Separator();
		for (const std::string& typeName : typeNames)
		{
			const auto* pEntry = CE::ComponentFactory::GetTypeInfo(typeName);
			bool alreadyPresent = pEntry && !pEntry->allowMultiple && CE::ComponentFactory::HasComponentOfType(pGameObject, typeName);
			if (ImGui::MenuItem(typeName.c_str(), nullptr, false, !alreadyPresent) && pEntry && pEntry->defaultCreator)
			{
				std::unique_ptr<CE::Component> pNewComponent(pEntry->defaultCreator());
				if (pNewComponent)
				{
					CE::Component* pComponent = pNewComponent.get();
					pGameObject->AddComponent(std::move(pNewComponent));
					pComponent->Initialize();
				}
			}
		}
	}
	ImGui::EndPopup();
}
