/*------------------------------
| File: Scene.cpp
| Author: Chandler Mays
------------------------------*/
#include "Scene.h"

#include "Core/GameObject.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*---------------------------------------------------------------
| --- Constructor: Constructs the Scene with default values --- |
---------------------------------------------------------------*/
CE::Scene::Scene()
	: m_name{ "Scene" }
	, m_isActive{ true }
	, m_isDirty{ false }
	, m_isLoaded{ false }
{}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::Scene::~Scene()
{
	Clear();
}

/*----------------------------------------------------------------
| --- Initialize: Initializes this Scene and its GameObjects --- |
----------------------------------------------------------------*/
bool CE::Scene::Initialize()
{
	for (const auto& pObject : m_gameObjects)
	{
		if (pObject != nullptr && !pObject->Initialize())
		{
			return false;
		}
	}
	m_isLoaded = true;
	return true;
}

/*--------------------------------------------------------
| --- Update: Updates this Scene and its GameObjects --- |
--------------------------------------------------------*/
void CE::Scene::Update(float deltaTime)
{
	for (const auto& pObject : m_gameObjects)
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
	for (const auto& pObject : m_gameObjects)
	{
		if (pObject && pObject->IsActive())
		{
			pObject->Render();
		}
	}
}

/*-------------------------------------------------
| --- GetName: Returns the name of this Scene --- |
-------------------------------------------------*/
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

/*---------------------------------------------------------------
| --- IsActive: Returns whether this Scene is active or not --- |
---------------------------------------------------------------*/
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
	return m_isLoaded && !m_gameObjects.empty();
}

/*--------------------------------------------------------
| --- AddGameObject: Adds a GameObject to this Scene --- |
--------------------------------------------------------*/
void CE::Scene::AddGameObject(std::unique_ptr<GameObject> pGameObject)
{
	if (pGameObject)
	{
		m_gameObjects.emplace_back(std::move(pGameObject));
	}
}

/*----------------------------------------------------------------
| --- RemoveGameObject: Removes a GameObject from this Scene --- |
----------------------------------------------------------------*/
void CE::Scene::RemoveGameObject(GameObject* pGameObject)
{
	auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
		[pGameObject](const std::unique_ptr<GameObject>& p)
		{
			return p.get() == pGameObject;
		});

	if (it != m_gameObjects.end())
		m_gameObjects.erase(it);
}

/*----------------------------------------------------------------------------------------------
| --- GetGameObjects: Returns a vector of unique pointers to the GameObjects in this Scene --- |
----------------------------------------------------------------------------------------------*/
const std::vector<std::unique_ptr<CE::GameObject>>& CE::Scene::GetGameObjects() const
{
	return m_gameObjects;
}

/*-------------------------------------------------------
| --- Clear: Clears all GameObjects from this Scene --- |
-------------------------------------------------------*/
void CE::Scene::Clear()
{
	m_gameObjects.clear();
}