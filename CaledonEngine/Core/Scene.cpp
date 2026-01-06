#include "Scene.h"

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
}

/*----------------------------------------------------------------
| --- Initialize: Initializes this Scene and its GameObjects --- |
----------------------------------------------------------------*/
bool CE::Scene::Initialize()
{
	return false;
}

/*--------------------------------------------------------
| --- Update: Updates this Scene and its GameObjects --- |
--------------------------------------------------------*/
void CE::Scene::Update(float deltaTime)
{
}

/*--------------------------------------------------------
| --- Render: Renders this Scene and its GameObjects --- |
--------------------------------------------------------*/
void CE::Scene::Render()
{
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