/*------------------------------
| File: GameObject.cpp
| Author: Chandler Mays
------------------------------*/
#include "GameObject.h"

#include "Systems/Engine/LoggingManager.h"
#include "Core/Transform.h"
#include "Core/Component.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------
| --- Constructor: Constructs the GameObject with default values --- |
--------------------------------------------------------------------*/
CE::GameObject::GameObject()
	: m_pParent{ nullptr }
	, m_name{ "GameObject" }
	, m_tag{ "Untagged" }
	, m_isActive{ true }
	, m_pTransform{ nullptr }
{
	// Create a new Transform component and add it to the GameObject
	m_pTransform = new Transform();
	AddComponent(m_pTransform);
}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::GameObject::~GameObject()
{
	Destroy();
}

/*--------------------------------------------------------------------
| --- Initialize: Initializes this GameObject and its components --- |
--------------------------------------------------------------------*/
bool CE::GameObject::Initialize()
{
	bool success = true;
	for (Component* pComponent : m_components)
	{
		if (pComponent != nullptr && !pComponent->Initialize())
		{
			CE_LOG("GameObject::Initialize - A component on '" + m_name + "' failed to initialize.");
			success = false;
		}
	}
	return success;
}

/*------------------------------------------------------------
| --- Update: Updates this GameObject and its components --- |
------------------------------------------------------------*/
void CE::GameObject::Update(float deltaTime)
{
	if (!m_isActive)
		return;

	for (Component* pComponent : m_components)
	{
		if (pComponent != nullptr && pComponent->IsActive())
		{
			pComponent->Update(deltaTime);
		}
	}

	for (GameObject* child : m_children)
	{
		if (child != nullptr && child->IsActive())
		{
			child->Update(deltaTime);
		}
	}
}

/*------------------------------------------------------------
| --- Render: Renders this GameObject and its components --- |
------------------------------------------------------------*/
void CE::GameObject::Render()
{
	if (!m_isActive)
		return;

	for (Component* pComponent : m_components)
	{
		if (pComponent != nullptr && pComponent->IsActive())
		{
			pComponent->Render();
		}
	}
	for (GameObject* child : m_children)
	{
		if (child != nullptr && child->IsActive())
		{
			child->Render();
		}
	}
}

/*---------------------------------------------------------------
| --- GetParent: Gets a pointer to this GameObject's parent --- |
---------------------------------------------------------------*/
CE::GameObject* CE::GameObject::GetParent() const
{
    return m_pParent;
}

/*-------------------------------------------------------
| --- SetParent: Sets the parent of this GameObject --- |
-------------------------------------------------------*/
void CE::GameObject::SetParent(GameObject* pParent)
{
    m_pParent = pParent;
}

/*------------------------------------------------------------------------------
| --- GetChildren: Gets a vector of pointers to this GameObject's children --- |
------------------------------------------------------------------------------*/
const std::vector<CE::GameObject*>& CE::GameObject::GetChildren() const
{
    return m_children;
}

/*---------------------------------------------------
| --- AddChild: Adds a child to this GameObject --- |
---------------------------------------------------*/
void CE::GameObject::AddChild(GameObject* pChild)
{
	if (pChild != nullptr)
	{
		m_children.emplace_back(pChild);
		pChild->SetParent(this);
	}
}

/*-----------------------------------------------------------
| --- RemoveChild: Removes a child from this GameObject --- |
-----------------------------------------------------------*/
void CE::GameObject::RemoveChild(GameObject* pChild)
{
	auto it = std::find(m_children.begin(), m_children.end(), pChild);
	if (it != m_children.end())
	{
		(*it)->SetParent(nullptr);
		m_children.erase(it);
	}
}

/*---------------------------------------------------
| --- GetName: Gets the name of this GameObject --- |
---------------------------------------------------*/
const std::string& CE::GameObject::GetName() const
{
	return m_name;
}

/*---------------------------------------------------
| --- SetName: Sets the name of this GameObject --- |
---------------------------------------------------*/
void CE::GameObject::SetName(const std::string& name)
{
	m_name = name;
}

/*------------------------------------------------
| --- GetTag: Gets the tag of the GameObject --- |
------------------------------------------------*/
const std::string& CE::GameObject::GetTag() const
{
	return m_tag;
}

/*-------------------------------------------------
| --- SetTag: Sets the tag of this GameObject --- |
-------------------------------------------------*/
void CE::GameObject::SetTag(const std::string& tag)
{
	m_tag = tag;
}

/*-----------------------------------------------------------------
| --- IsActive: Gets whether this GameObject is active or not --- |
-----------------------------------------------------------------*/
bool CE::GameObject::IsActive() const
{
	return m_isActive;
}

/*------------------------------------------------------------------
| --- SetActive: Sets whether this GameObject is active or not --- |
------------------------------------------------------------------*/
void CE::GameObject::SetActive(bool isActive)
{
	m_isActive = isActive;
}

/*-------------------------------------------------------------
| --- GetTransform: Gets the Transform of this GameObject --- |
-------------------------------------------------------------*/
const CE::Transform& CE::GameObject::GetTransform() const
{
	return *m_pTransform;
}

/*-------------------------------------------------------------
| --- GetTransform: Gets the Transform of this GameObject --- |
-------------------------------------------------------------*/
CE::Transform& CE::GameObject::GetTransform()
{
	return *m_pTransform;
}

/*-----------------------------------------------------------
| --- AddComponent: Adds a component to this GameObject --- |
-----------------------------------------------------------*/
void CE::GameObject::AddComponent(CE::Component* pComponent)
{
	if (pComponent != nullptr)
	{
		m_components.emplace_back(pComponent);
		pComponent->SetOwner(this);
	}
}

/*-------------------------------------------------------------------
| --- RemoveComponent: Removes a component from this GameObject --- |
-------------------------------------------------------------------*/
void CE::GameObject::RemoveComponent(CE::Component* pComponent)
{
	if (pComponent == nullptr || pComponent == m_pTransform)
		return;

	auto it = std::find(m_components.begin(), m_components.end(), pComponent);
	if (it != m_components.end())
	{
		(*it)->SetOwner(nullptr);
		m_components.erase(it);
	}
}

/*--------------------------------------------------------------
| --- Destroy: Destroys this GameObject and its components --- |
--------------------------------------------------------------*/
void CE::GameObject::Destroy()
{
	for (Component* pComponent : m_components)
	{
		delete pComponent;
		pComponent = nullptr;
	}
	m_components.clear();
	m_pTransform = nullptr;

	for (GameObject* child : m_children)
	{
		delete child;
		child = nullptr;
	}
	m_children.clear();
}