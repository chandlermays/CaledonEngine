/*------------------------------
| File: HierarchyPanel.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <string>

namespace CE
{
	class GameObject;
}

class HierarchyPanel
{
private:
	int m_createdCount;														// Counter for the number of GameObjects created in the current session

	void DrawGameObjectNode(CE::GameObject* pGameObject);					// Draws a single GameObject node in the hierarchy

public:
	HierarchyPanel();														// Constructor
	~HierarchyPanel() = default;											// Destructor
	HierarchyPanel(const HierarchyPanel&) = delete;							// Prevent copy-construction
	HierarchyPanel& operator=(const HierarchyPanel&) = delete;				// Prevent copy-assignment
	HierarchyPanel(HierarchyPanel&&) = delete;								// Prevent move-construction
	HierarchyPanel& operator=(HierarchyPanel&&) = delete;					// Prevent move-assignment

	void Draw();															// Draws the Hierarchy panel to create and store GameObjects in the current scene
};