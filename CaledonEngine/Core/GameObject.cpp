#include "GameObject.h"

/*---------------------
| --- Constructor --- |
---------------------*/
CE::GameObject::GameObject()
	: m_pParent{ nullptr }
	, m_name{ "GameObject" }
	, m_tag{ "Untagged" }
	, m_isActive{ true }
{}

/*--------------------
| --- Destructor --- |
--------------------*/
CE::GameObject::~GameObject()
{
	Destroy();
}

/*--------------------------------------------------------------------
| --- Initialize: Initializes this GameObject and its components --- |
--------------------------------------------------------------------*/
bool CE::GameObject::Initialize()
{
	return false;
}

/*------------------------------------------------------------
| --- Update: Updates this GameObject and its components --- |
------------------------------------------------------------*/
void CE::GameObject::Update(float deltaTime)
{
}

/*------------------------------------------------------------
| --- Render: Renders this GameObject and its components --- |
------------------------------------------------------------*/
void CE::GameObject::Render()
{
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
void CE::GameObject::SetParent(GameObject* parent)
{
    m_pParent = parent;
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
void CE::GameObject::AddChild(GameObject* child)
{
}

/*-----------------------------------------------------------
| --- RemoveChild: Removes a child from this GameObject --- |
-----------------------------------------------------------*/
void CE::GameObject::RemoveChild(GameObject* child)
{
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
	return m_transform;
}

/*-----------------------------------------------------------
| --- AddComponent: Adds a component to this GameObject --- |
-----------------------------------------------------------*/
void CE::GameObject::AddComponent(CE::Component* component)
{
}

/*-------------------------------------------------------------------
| --- RemoveComponent: Removes a component from this GameObject --- |
-------------------------------------------------------------------*/
void CE::GameObject::RemoveComponent(CE::Component* component)
{
}

/*--------------------------------------------------------------
| --- Destroy: Destroys this GameObject and its components --- |
--------------------------------------------------------------*/
void CE::GameObject::Destroy()
{
}