/*------------------------------
| File: Collider2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "Collider2D.h"
#include "Systems/Engine/EngineManager.h"
#include "Systems/Physics/CollisionManager.h"

#include <algorithm>
#include <limits>

namespace
{
	/*-------------------------------------------------------------------------------------------
	| --- Flip: Re-expresses a separation from the other collider's point of view (A <-> B) --- |
	-------------------------------------------------------------------------------------------*/

	CE::ColliderDistance2D Flip(const CE::ColliderDistance2D& distance)
	{
		if (!distance.isValid)
			return distance;

		return CE::ColliderDistance2D(distance.pointB, distance.pointA, -distance.normal, distance.distance, distance.isOverlapped);
	}
}

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------
| --- Constructor: Constructs the Collider2D with default values --- |
--------------------------------------------------------------------*/
CE::Collider2D::Collider2D()
	: Component()
	, m_isTrigger{ false }
	, m_pAttachedMaterial{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::Collider2D::~Collider2D()
{
	auto* pCollisionManager = EngineManager::GetInstance().GetCollisionManager();
	if (pCollisionManager)
	{
		pCollisionManager->RemoveActiveCollider(this);
	}
}

/*-----------------------------------------------------
| --- Initialize: Prepares the Collider2D for use --- |
-----------------------------------------------------*/
bool CE::Collider2D::Initialize()
{
	RecalculateBounds();

	auto* pCollisionManager = EngineManager::GetInstance().GetCollisionManager();
	if (pCollisionManager)
	{
		pCollisionManager->AddActiveCollider(this);
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------------------
| --- Update: Updates the Collider2D's bounds based on the GameObject's transform --- |
-------------------------------------------------------------------------------------*/
void CE::Collider2D::Update(float)
{
	RecalculateBounds();
}

/*--------------------------------------------------------------------------------------------
| --- RefreshBounds: Updates the Collider2D's bounds based on the GameObject's transform --- |
--------------------------------------------------------------------------------------------*/
void CE::Collider2D::RefreshBounds()
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

/*----------------------------------------------------------------------------------------------------
| --- OverlapPoint: Returns true if a given point in world space is inside the collider's bounds --- |
----------------------------------------------------------------------------------------------------*/
bool CE::Collider2D::OverlapPoint(const Vector2f& point) const
{
	return Vector2f::SqrDistance(point, ClosestPoint(point)) <= 0.0f;
}

/*----------------------------------------------------------------------------------------------------------------------------
| --- Distance: Returns the distance, closest points, and overlap information between this collider and another collider --- |
----------------------------------------------------------------------------------------------------------------------------*/
CE::ColliderDistance2D CE::Collider2D::Distance(const Collider2D& other) const
{
	if (Overlaps(other))
	{
		if (IsAxisAlignedBox() && !other.IsAxisAlignedBox())
		{
			return Flip(other.ResolveOverlapDistance(*this));
		}
		return ResolveOverlapDistance(other);
	}

	Vector2f thisPoint = GetBounds().GetCenter();
	Vector2f otherPoint = other.GetBounds().GetCenter();

	constexpr int kClosestPointIterations = 4;
	for (int i = 0; i < kClosestPointIterations; ++i)
	{
		thisPoint = ClosestPoint(otherPoint);
		otherPoint = other.ClosestPoint(thisPoint);
	}

	float distance = Vector2f::Distance(thisPoint, otherPoint);
	Vector2f normal = (distance > std::numeric_limits<float>::epsilon())
		? (thisPoint - otherPoint) / distance
		: Vector2f::Zero();

	return ColliderDistance2D(thisPoint, otherPoint, normal, distance, false);
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

/*--------------------------------------------------------------------------------------------------------------------
| --- GetFriction: Returns the friction of a collider; used to control how a collision response reduces velocity --- |
--------------------------------------------------------------------------------------------------------------------*/
float CE::Collider2D::GetFriction() const
{
	return ResolveMaterial().GetFriction();
}

/*--------------------------------------------------------------------------------------------------------------------
| --- GetBounciness: Returns the bounciness of a collider; used to control how "elastic" a collision response is --- |
--------------------------------------------------------------------------------------------------------------------*/
float CE::Collider2D::GetBounciness() const
{
	return ResolveMaterial().GetBounciness();
}

/*---------------------------------------------------------------------------------------------------------------------
| --- GetFrictionCombo: Determines how the effective friction is calculated when two Collider2D come into contact --- |
---------------------------------------------------------------------------------------------------------------------*/
CE::PhysicsMaterialCombine2D CE::Collider2D::GetFrictionCombine() const
{
	return ResolveMaterial().GetFrictionCombine();
}

/*---------------------------------------------------------------------------------------------------------------------
| --- GetBounceCombo: Determines how the effective bounciness is calculated when two Collider2D come into contact --- |
---------------------------------------------------------------------------------------------------------------------*/
CE::PhysicsMaterialCombine2D CE::Collider2D::GetBounceCombine() const
{
	return ResolveMaterial().GetBounceCombine();
}

/*-------------------------------------------------------------------------------------------
| --- GetSharedMaterial: Returns the PhysicsMaterial2D that is applied to this collider --- |
-------------------------------------------------------------------------------------------*/
CE::PhysicsMaterial2D* CE::Collider2D::GetSharedMaterial() const
{
	return m_pAttachedMaterial;
}

/*-------------------------------------------------------------------------------------
| --- SetSharedMaterial: Sets a PhysicsMaterial2D to be assigned to this collider --- |
-------------------------------------------------------------------------------------*/
void CE::Collider2D::SetSharedMaterial(PhysicsMaterial2D* pMaterial)
{
	m_pAttachedMaterial = pMaterial;
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



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------------
| --- ResolveMaterial: Returns the effective PhysicsMaterial2D for this collider --- |
------------------------------------------------------------------------------------*/
const CE::PhysicsMaterial2D& CE::Collider2D::ResolveMaterial() const
{
	if (m_pAttachedMaterial)
		return *m_pAttachedMaterial;

	// TODO: Once RigidBody2D exists, check m_pOwner's attached RigidBody2D for a material here
	
	return PhysicsMaterial2D::GetDefault();
}



/*--------------------------------------
| --- Protected Method Definitions --- |
--------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------
| --- ResolveOverlapDistance: Returns the distance, closest points, and overlap information between this collider and another collider --- |
------------------------------------------------------------------------------------------------------------------------------------------*/
CE::ColliderDistance2D CE::Collider2D::ResolveOverlapDistance(const Collider2D& other) const
{
	const AABB2D& boundsA = GetBounds();
	const AABB2D& boundsB = other.GetBounds();

	const float overlapMinX = std::max(boundsA.min.x, boundsB.min.x);
	const float overlapMaxX = std::min(boundsA.max.x, boundsB.max.x);
	const float overlapMinY = std::max(boundsA.min.y, boundsB.min.y);
	const float overlapMaxY = std::min(boundsA.max.y, boundsB.max.y);

	const float overlapX = overlapMaxX - overlapMinX;
	const float overlapY = overlapMaxY - overlapMinY;

	if (overlapX < 0.0f || overlapY < 0.0f)
		return ColliderDistance2D();
	
	const Vector2f centerA = boundsA.GetCenter();
	const Vector2f centerB = boundsB.GetCenter();

	// Separate along the axis of least overlap; the points sit mid-way along the shared span of the other axis
	const float midX = (overlapMinX + overlapMaxX) * 0.5f;
	const float midY = (overlapMinY + overlapMaxY) * 0.5f;

	Vector2f normal;
	Vector2f pointA;
	Vector2f pointB;
	float depth;

	if (overlapX < overlapY)
	{
		depth = overlapX;
		const bool thisIsRight = centerA.x >= centerB.x;
		normal = { thisIsRight ? 1.0f : -1.0f, 0.0f };
		pointA = { thisIsRight ? boundsA.min.x : boundsA.max.x, midY };
		pointB = { thisIsRight ? boundsB.max.x : boundsB.min.x, midY };
	}
	else
	{
		depth = overlapY;
		const bool thisIsAbove = centerA.y >= centerB.y;
		normal = { 0.0f, thisIsAbove ? 1.0f : -1.0f };
		pointA = { midX, thisIsAbove ? boundsA.min.y : boundsA.max.y };
		pointB = { midX, thisIsAbove ? boundsB.max.y : boundsB.min.y };
	}

	return ColliderDistance2D(pointA, pointB, normal, -depth, depth > 0.0f);
}