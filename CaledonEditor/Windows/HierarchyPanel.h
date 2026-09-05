#pragma once
#include <string>

namespace CE
{
	class GameObject;
}

class HierarchyPanel
{
private:
	int m_createdCount;

	void DrawGameObjectNode(CE::GameObject* pGameObject);

public:
	HierarchyPanel();

	void Draw();
};