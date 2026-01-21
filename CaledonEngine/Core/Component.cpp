#include "Component.h"

/*---------------------------------------------------------------------------------------------------------------
| --- NOTE: If this class has nothing more than one-line defintions, consider making it a header-only class --- |
---------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------
| --- Constructor: Constructs the Component with default values --- |
-------------------------------------------------------------------*/
CE::Component::Component()
	: m_pOwner{ nullptr }
	, m_isActive{ true }
{}

/*-----------------------------------------------------------
| --- GetOwner: Gets a pointer to the owning GameObject --- |
-----------------------------------------------------------*/
CE::GameObject* CE::Component::GetOwner() const
{
	return m_pOwner;
}

/*----------------------------------------------
| --- SetOwner: Sets the owning GameObject --- |
----------------------------------------------*/
void CE::Component::SetOwner(GameObject* owner)
{
	m_pOwner = owner;
}

/*------------------------------------------------------------
| --- GetID: Gets the unique identifier of the Component --- |
------------------------------------------------------------*/
const std::string& CE::Component::GetID() const
{
	return m_id;
}

/*------------------------------------------------------------
| --- SetID: Sets the unique identifier of the Component --- |
------------------------------------------------------------*/
void CE::Component::SetID(const std::string& id)
{
	m_id = id;
}

/*---------------------------------------------------------------
| --- IsActive: Gets whether the Component is active or not --- |
---------------------------------------------------------------*/
bool CE::Component::IsActive() const
{
	return m_isActive;
}

/*----------------------------------------------------------------
| --- SetActive: Sets whether the Component is active or not --- |
----------------------------------------------------------------*/
void CE::Component::SetActive(bool isActive)
{
	m_isActive = isActive;
}