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

	void Draw();															// Draws the Hierarchy panel to create and store GameObjects in the current scene
};