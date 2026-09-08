/*------------------------------
| File: InspectorPanel.h
| Author: Chandler Mays
------------------------------*/
#pragma once

namespace CE
{
	class GameObject;
	class Component;
	struct PropertyDescriptor;
}
class EditorContext;

class InspectorPanel
{
private:
	void DrawComponent(CE::Component* pComponent);															// Draws the properties of a single component in the Inspector panel
	void DrawProperty(CE::Component* pComponent, const CE::PropertyDescriptor& property);					// Draws a single property of a component in the Inspector panel
	void DrawAddComponentMenu(CE::GameObject* pGameObject);													// Draws the "Add Component" button and menu in the Inspector panel

public:
	InspectorPanel() = default;																				// Constructor
	~InspectorPanel() = default;																			// Destructor
	InspectorPanel(const InspectorPanel&) = delete;															// Prevent copy-construction
	InspectorPanel& operator=(const InspectorPanel&) = delete;												// Prevent copy-assignment
	InspectorPanel(InspectorPanel&&) = delete;																// Prevent move-construction
	InspectorPanel& operator=(InspectorPanel&&) = delete;													// Prevent move-assignment

	void Draw(EditorContext& context);																		// Draws the Inspector panel to view and edit properties of the selected GameObject
};