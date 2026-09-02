/*------------------------------
| File: Collider2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "Collider2D.h"
#include "Systems/Engine/EngineManager.h"
// #include "Systems/Physics/CollisionManager.h"		// once it exists — see "what's next" below

/*--------------------------------------------------------------------
| --- Constructor: Constructs the Collider2D with default values --- |
--------------------------------------------------------------------*/
CE::Collider2D::Collider2D()
	: Component()
	, m_isTrigger{ false }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::Collider2D::~Collider2D()
{
	// Unregister defensively — EngineManager may already be mid-shutdown, in which case
	// GetCollisionManager() returns nullptr rather than a stale pointer.
	// auto* pCollisionManager = EngineManager::GetInstance().GetCollisionManager();
	// if (pCollisionManager)
	// {
	// 	pCollisionManager->RemoveActiveCollider(this);
	// }
}

/*-----------------------------------------------------
| --- Initialize: Prepares the Collider2D for use --- |
-----------------------------------------------------*/
bool CE::Collider2D::Initialize()
{
	RecalculateBounds();

	// auto* pCollisionManager = EngineManager::GetInstance().GetCollisionManager();
	// if (pCollisionManager)
	// {
	// 	pCollisionManager->AddActiveCollider(this);
	// 	return true;
	// }
	// return false;

	return true;		// placeholder until CollisionManager exists
}

/*-------------------------------------------------------------------------------------
| --- Update: Updates the Collider2D's bounds based on the GameObject's transform --- |
-------------------------------------------------------------------------------------*/
void CE::Collider2D::Update(float)
{
	RecalculateBounds();
}

/*-------------------------------------------------------------------------------------------------------------------------
| --- Distance: Returns the distance from a given point in world space to the closest point on the collider's surface --- |
-------------------------------------------------------------------------------------------------------------------------*/
float CE::Collider2D::Distance(const Vector2f& point) const
{
	return Vector2f::Distance(point, ClosestPoint(point));
}

/*------------------------------------------------------------------------------------------------
| --- GetBounds: Returns the Axis-Aligned Bounding Box (AABB) of the collider in world space --- |
------------------------------------------------------------------------------------------------*/
const CE::AABB2D& CE::Collider2D::GetBounds() const
{
	return m_bounds;
}

/*----------------------------------------------------------------------------------------------------------------
| --- IsTrigger: Returns whether the collider is a trigger (does not cause physical collisions, only events) --- |
----------------------------------------------------------------------------------------------------------------*/
bool CE::Collider2D::IsTrigger() const
{
	return m_isTrigger;
}

/*--------------------------------------------------------------------------------------------------------------
| --- SetTrigger: Sets whether the collider is a trigger (does not cause physical collisions, only events) --- |
--------------------------------------------------------------------------------------------------------------*/
void CE::Collider2D::SetTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;
}

/*------------------------------------------------------------------------------------------------------
| --- GetOffset: Returns the offset of the collider's bounds relative to the GameObject's position --- |
------------------------------------------------------------------------------------------------------*/
const CE::Vector2f& CE::Collider2D::GetOffset() const
{
	return m_offset;
}

/*-------------------------------------------------------------------------------------------------------------------------------
| --- SetOffset: Sets the offset of the collider's bounds relative to the GameObject's position and recalculates the bounds --- |
-------------------------------------------------------------------------------------------------------------------------------*/
void CE::Collider2D::SetOffset(const Vector2f& offset)
{
	m_offset = offset;
	RecalculateBounds();
}

/*--------------------------------------------------------------------------------------
| --- OnCollisionEnter: Registers a callback to be invoked when a collision starts --- |
--------------------------------------------------------------------------------------*/
void CE::Collider2D::OnCollisionEnter(CollisionCallback callback)
{
	m_onEnterCallbacks.push_back(std::move(callback));
}

/*-------------------------------------------------------------------------------------------
| --- OnCollisionUpdate: Registers a callback to be invoked when a collision is ongoing --- |
-------------------------------------------------------------------------------------------*/
void CE::Collider2D::OnCollisionUpdate(CollisionCallback callback)
{
	m_onUpdateCallbacks.push_back(std::move(callback));
}

/*-----------------------------------------------------------------------------------
| --- OnCollisionExit: Registers a callback to be invoked when a collision ends --- |
-----------------------------------------------------------------------------------*/
void CE::Collider2D::OnCollisionExit(CollisionCallback callback)
{
	m_onExitCallbacks.push_back(std::move(callback));
}

/*--------------------------------------------------------------------------------------------------------------
| --- InvokeEnter: Invokes all registered collision enter callbacks with the other collider as an argument --- |
--------------------------------------------------------------------------------------------------------------*/
void CE::Collider2D::InvokeEnter(Collider2D* pOther)
{
	for (const auto& callback : m_onEnterCallbacks) { callback(pOther); }
}

/*----------------------------------------------------------------------------------------------------------------
| --- InvokeUpdate: Invokes all registered collision update callbacks with the other collider as an argument --- |
----------------------------------------------------------------------------------------------------------------*/
void CE::Collider2D::InvokeUpdate(Collider2D* pOther)
{
	for (const auto& callback : m_onUpdateCallbacks) { callback(pOther); }
}

/*------------------------------------------------------------------------------------------------------------
| --- InvokeExit: Invokes all registered collision exit callbacks with the other collider as an argument --- |
------------------------------------------------------------------------------------------------------------*/
void CE::Collider2D::InvokeExit(Collider2D* pOther)
{
	for (const auto& callback : m_onExitCallbacks) { callback(pOther); }
}