#include "Scene.h"
#include "CaledonEngine/Core/GameObject.h"

/*---------------------
| --- Constructor --- |
---------------------*/
CE::Scene::Scene()
	: m_name{ "Scene" }
	, m_isActive{ true }
{}

/*--------------------
| --- Destructor --- |
--------------------*/
CE::Scene::~Scene()
{
	Clear();
}

/*----------------------------------------------------------------
| --- Initialize: Initializes this Scene and its GameObjects --- |
----------------------------------------------------------------*/
bool CE::Scene::Initialize()
{
	for (GameObject* pObject : m_gameObjects)
	{
		if (pObject != nullptr && !pObject->Initialize())
		{
			return false;
		}
	}
	return true;
}

/*--------------------------------------------------------
| --- Update: Updates this Scene and its GameObjects --- |
--------------------------------------------------------*/
void CE::Scene::Update(float deltaTime)
{
	for (GameObject* pObject : m_gameObjects)
	{
		if (pObject->IsActive())
		{
			pObject->Update(deltaTime);
		}
	}
}

/*--------------------------------------------------------
| --- Render: Renders this Scene and its GameObjects --- |
--------------------------------------------------------*/
void CE::Scene::Render()
{
	for (GameObject* pObject : m_gameObjects)
	{
		if (pObject->IsActive())
		{
			pObject->Render();
		}
	}
}

/*----------------------------------------------
| --- GetName: Gets the name of this Scene --- |
----------------------------------------------*/
const std::string& CE::Scene::GetName() const
{
	return m_name;
}

/*----------------------------------------------
| --- SetName: Sets the name of this Scene --- |
----------------------------------------------*/
void CE::Scene::SetName(const std::string& name)
{
	m_name = name;
}

/*------------------------------------------------------------
| --- IsActive: Gets whether this Scene is active or not --- |
------------------------------------------------------------*/
bool CE::Scene::IsActive() const
{
	return m_isActive;
}

/*-------------------------------------------------------------
| --- SetActive: Sets whether this Scene is active or not --- |
-------------------------------------------------------------*/
void CE::Scene::SetActive(bool isActive)
{
	m_isActive = isActive;
}

/*------------------------------------------------
| --- IsValid: Checks if this Scene is valid --- |
------------------------------------------------*/
bool CE::Scene::IsValid() const
{
	return false;
}

/*--------------------------------------------------------
| --- AddGameObject: Adds a GameObject to this Scene --- |
--------------------------------------------------------*/
void CE::Scene::AddGameObject(GameObject* pGameObject)
{
	m_gameObjects.emplace_back(pGameObject);
}

/*----------------------------------------------------------------
| --- RemoveGameObject: Removes a GameObject from this Scene --- |
----------------------------------------------------------------*/
void CE::Scene::RemoveGameObject(GameObject* pGameObject)
{
	auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), pGameObject);
	if (it != m_gameObjects.end())
	{
		m_gameObjects.erase(it);
	}
}

/*------------------------------------------------------------
| --- GetGameObjects: Gets the GameObjects in this Scene --- |
------------------------------------------------------------*/
const std::vector<CE::GameObject*>& CE::Scene::GetGameObjects() const
{
	return m_gameObjects;
}

/*-------------------------------------------------------
| --- Clear: Clears all GameObjects from this Scene --- |
-------------------------------------------------------*/
void CE::Scene::Clear()
{
	for (GameObject* pObject : m_gameObjects)
	{
		delete pObject;
		pObject = nullptr;
	}
	m_gameObjects.clear();
}