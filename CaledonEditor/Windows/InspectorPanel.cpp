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

#include <CaledonEngine/lib/ImGUI/imgui.h>

#include <unordered_map>
#include <vector>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------------------------------
| --- Draw: Draws the Inspector panel to view and edit properties of the selected GameObject --- |
------------------------------------------------------------------------------------------------*/
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
	{
		pGameObject->SetName(nameBuffer);
	}

	ImGui::Separator();

	for (CE::Component* pComponent : pGameObject->GetAllComponents())
	{
		DrawComponent(pComponent);
	}

	ImGui::Separator();

	DrawAddComponentMenu(pGameObject);

	ImGui::End();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------------------
| --- DrawComponent: Draws the properties of a single component in the Inspector panel --- |
------------------------------------------------------------------------------------------*/
void InspectorPanel::DrawComponent(CE::Component* pComponent)
{
	if (!pComponent)
		return;

	const std::string& typeName = pComponent->GetTypeName();
	const CE::ComponentFactory::RegistryEntry* pTypeInfo = CE::ComponentFactory::GetTypeInfo(typeName);

	ImGui::PushID(pComponent);

	if (ImGui::CollapsingHeader(typeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!pTypeInfo || pTypeInfo->properties.empty())
		{
			ImGui::TextDisabled("(no editable properties)");
		}
		else
		{
			for (const CE::PropertyDescriptor& property : pTypeInfo->properties)
			{
				DrawProperty(pComponent, property);
			}
		}
	}

	ImGui::PopID();
}

/*-------------------------------------------------------------------------------------
| --- DrawProperty: Draws a single property of a component in the Inspector panel --- |
-------------------------------------------------------------------------------------*/
void InspectorPanel::DrawProperty(CE::Component* pComponent, const CE::PropertyDescriptor& property)
{
	CE::PropertyValue value = property.getter(pComponent);

	if (std::holds_alternative<float>(value))
	{
		float v = std::get<float>(value);
		if (ImGui::DragFloat(property.name.c_str(), &v))
		{
			property.setter(pComponent, v);
		}
	}
	else if (std::holds_alternative<int>(value))
	{
		int v = std::get<int>(value);
		if (ImGui::DragInt(property.name.c_str(), &v))
		{
			property.setter(pComponent, v);
		}
	}
	else if (std::holds_alternative<bool>(value))
	{
		bool v = std::get<bool>(value);
		if (ImGui::Checkbox(property.name.c_str(), &v))
		{
			property.setter(pComponent, v);
		}
	}
	else if (std::holds_alternative<CE::Vector2f>(value))
	{
		CE::Vector2f v = std::get<CE::Vector2f>(value);
		float components[2] = { v.x, v.y };
		if (ImGui::DragFloat2(property.name.c_str(), components))
		{
			property.setter(pComponent, CE::Vector2f(components[0], components[1]));
		}
	}
	else if (std::holds_alternative<CE::Color>(value))
	{
		CE::Color v = std::get<CE::Color>(value);
		float components[4] = { v.r / 255.0f, v.g / 255.0f, v.b / 255.0f, v.a / 255.0f };
		if (ImGui::ColorEdit4(property.name.c_str(), components))
		{
			CE::Color newColor(
				static_cast<std::uint8_t>(components[0] * 255.0f),
				static_cast<std::uint8_t>(components[1] * 255.0f),
				static_cast<std::uint8_t>(components[2] * 255.0f),
				static_cast<std::uint8_t>(components[3] * 255.0f));
			property.setter(pComponent, newColor);
		}
	}
	else if (std::holds_alternative<std::string>(value))
	{
		std::string v = std::get<std::string>(value);
		char buffer[256];
		strncpy_s(buffer, v.c_str(), sizeof(buffer) - 1);
		if (ImGui::InputText(property.name.c_str(), buffer, sizeof(buffer)))
		{
			property.setter(pComponent, std::string(buffer));
		}
	}
}

/*--------------------------------------------------------------------------------------------
| --- DrawAddComponent: Draws the "Add Component" button and menu in the Inspector panel --- |
--------------------------------------------------------------------------------------------*/
void InspectorPanel::DrawAddComponentMenu(CE::GameObject* pGameObject)
{
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponentPopup");
	}

	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		std::unordered_map<std::string, std::vector<std::string>> categorized;
		for (const auto& [typeName, entry] : CE::ComponentFactory::GetAllRegisteredTypes())
		{
			if (entry.defaultCreator)		// Types with no default constructor (e.g. Transform) aren't offered here
			{
				categorized[entry.category].push_back(typeName);
			}
		}

		for (const auto& [category, typeNames] : categorized)
		{
			ImGui::Text("%s", category.c_str());
			ImGui::Separator();

			for (const std::string& typeName : typeNames)
			{
				if (ImGui::MenuItem(typeName.c_str()))
				{
					const CE::ComponentFactory::RegistryEntry* pEntry = CE::ComponentFactory::GetTypeInfo(typeName);
					if (pEntry && pEntry->defaultCreator)
					{
						CE::Component* pNewComponent = pEntry->defaultCreator();
						pGameObject->AddComponent(pNewComponent);
						pNewComponent->Initialize();
					}
				}
			}
		}

		ImGui::EndPopup();
	}
}