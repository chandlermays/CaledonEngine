#pragma once

namespace CE 
{
	class GameObject;
}

class EditorContext
{
private:
	CE::GameObject* m_pSelectedGameObject;

public:
	EditorContext()
		: m_pSelectedGameObject{ nullptr } 
	{ }

	void SetSelected(CE::GameObject* pGameObject)		{ m_pSelectedGameObject = pGameObject; }
	CE::GameObject* GetSelected() const					{ return m_pSelectedGameObject; }
	void ClearSelection()								{ m_pSelectedGameObject = nullptr; }
};