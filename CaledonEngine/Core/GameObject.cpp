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
	auto pTransform = std::make_unique<Transform>();
	m_pTransform = pTransform.get();
	AddComponent(std::move(pTransform));
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
	for (const auto& pComponent : m_components)
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

	for (const auto& pComponent : m_components)
	{
		if (pComponent != nullptr && pComponent->IsActive())
		{
			pComponent->Update(deltaTime);
		}
	}

	for (const auto& pChild : m_children)
	{
		if (pChild != nullptr && pChild->IsActive())
		{
			pChild->Update(deltaTime);
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

	for (const auto& pComponent : m_components)
	{
		if (pComponent != nullptr && pComponent->IsActive())
		{
			pComponent->Render();
		}
	}
	for (const auto& pChild : m_children)
	{
		if (pChild != nullptr && pChild->IsActive())
		{
			pChild->Render();
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
const std::vector<std::unique_ptr<CE::GameObject>>& CE::GameObject::GetChildren() const
{
	return m_children;
}

/*---------------------------------------------------
| --- AddChild: Adds a child to this GameObject --- |
---------------------------------------------------*/
void CE::GameObject::AddChild(std::unique_ptr<GameObject> pChild)
{
	if (pChild != nullptr)
	{
		pChild->SetParent(this);
		m_children.emplace_back(std::move(pChild));
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

/*---------------------------------------------------------------------------------------------------
| --- GetAllComponents: Gets a vector of pointers to all Components attached to this GameObject --- |
---------------------------------------------------------------------------------------------------*/
const std::vector<std::unique_ptr<CE::Component>>& CE::GameObject::GetAllComponents() const
{
	return m_components;
}

/*-----------------------------------------------------------
| --- AddComponent: Adds a component to this GameObject --- |
-----------------------------------------------------------*/
void CE::GameObject::AddComponent(std::unique_ptr<Component> pComponent)
{
	if (!pComponent)
		return;

	pComponent->SetOwner(this);
	m_components.emplace_back(std::move(pComponent));
}

/*-------------------------------------------------------------------
| --- RemoveComponent: Removes a component from this GameObject --- |
-------------------------------------------------------------------*/
void CE::GameObject::RemoveComponent(Component* pComponent)
{
	if (pComponent == nullptr || pComponent == m_pTransform)
		return;

	auto it = std::find_if(m_components.begin(), m_components.end(),
		[pComponent](const std::unique_ptr<Component>& p) { return p.get() == pComponent; });

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
	m_components.clear();
	m_pTransform = nullptr;

	m_children.clear();
}