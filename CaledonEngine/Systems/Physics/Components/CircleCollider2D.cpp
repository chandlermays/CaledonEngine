/*------------------------------
| File: CicleCollider2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "CircleCollider2D.h"

#include "Core/GameObject.h"
#include "Core/Transform.h"

namespace
{
	constexpr float kCenterInsideEpsilon = 1e-4f;		// A small epsilon value to determine if the center of the circle is inside another collider
}

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------------
| --- Constructor: Constructs the CircleCollider2D with default values --- |
--------------------------------------------------------------------------*/
CE::CircleCollider2D::CircleCollider2D()
	: Collider2D()
	, m_radius{ 0.5f }
{ }

/*------------------------------------------------------------------------------------------------------------------
| --- ClosestPoint: Returns the closest point on the circle collider's surface to a given point in world space --- |
------------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::CircleCollider2D::ClosestPoint(const Vector2f& point) const
{
	const Vector2f center = GetBounds().GetCenter();
	const Vector2f offset = point - center;
	const float sqrDistance = offset.SqrMagnitude();

	if (sqrDistance <= m_radius * m_radius)
		return point;

	return center + offset * (m_radius / std::sqrt(sqrDistance));
}

/*---------------------------------------------------------------------------------------
| --- Overlaps: Returns true if this circle collider overlaps with another collider --- |
---------------------------------------------------------------------------------------*/
bool CE::CircleCollider2D::Overlaps(const Collider2D& other) const
{
	// A circle overlaps a convex shape if the closest point on the convex shape to the circle's center is within the circle's radius

	const Vector2f center = m_bounds.GetCenter();
	const Vector2f closest = other.ClosestPoint(center);

	return Vector2f::SqrDistance(center, closest) <= m_radius * m_radius;
}

/*-----------------------------------------------------------
| --- SetRadius: Sets the radius of the circle collider --- |
-----------------------------------------------------------*/
void CE::CircleCollider2D::SetRadius(float radius)
{
	m_radius = radius;
}

/*--------------------------------------------------------------
| --- GetRadius: Returns the radius of the circle collider --- |
--------------------------------------------------------------*/
float CE::CircleCollider2D::GetRadius() const
{
	return m_radius;
}

/*-------------------------------------------------------------
| --- GetTypeName: Returns the type name of the Component --- |
-------------------------------------------------------------*/
const std::string& CE::CircleCollider2D::GetTypeName() const
{
	static const std::string typeName = "CircleCollider2D";
	return typeName;
}



/*--------------------------------------
| --- Protected Method Definitions --- |
--------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------
| --- RecalculateBounds: Recalculates the AABB based on the GameObject's transform and the circle collider's properties --- |
---------------------------------------------------------------------------------------------------------------------------*/
void CE::CircleCollider2D::RecalculateBounds()
{
	if (!m_pOwner)
		return;

	const Vector2f worldPosition = m_pOwner->GetTransform().GetPosition() + m_offset;
	m_bounds = AABB2D(worldPosition, m_radius);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------
| --- ResolveOverlapDistance: Returns the distance, closest points, and overlap information between this circle collider and another collider --- |
-------------------------------------------------------------------------------------------------------------------------------------------------*/
CE::ColliderDistance2D CE::CircleCollider2D::ResolveOverlapDistance(const Collider2D& other) const
{
	const Vector2f center = m_bounds.GetCenter();
	const Vector2f closest = other.ClosestPoint(center);
	const Vector2 toCenter = center - closest;
	const float sqrDistance = toCenter.SqrMagnitude();

	// Center outside the other shape: push out along the line from its closest surface point to the center
	if (sqrDistance > kCenterInsideEpsilon + kCenterInsideEpsilon)
	{
		const float distanceToSurface = std::sqrt(sqrDistance);
		const Vector2f normal = toCenter / distanceToSurface;
		const float distance = distanceToSurface - m_radius;

		return ColliderDistance2D(center - normal * m_radius, closest, normal, distance, distance < 0.0f);
	}

	// Center inside the other shape: escape through its nearest boundary
	const AABB2D& otherBounds = other.GetBounds();
	Vector2f normal;
	float exitDistance;

	if (other.IsAxisAlignedBox())
	{
		// Nearest face of the box
		exitDistance = center.x - otherBounds.min.x;
		normal = { -1.0f, 0.0f };

		if (otherBounds.max.x - center.x < exitDistance)
		{
			exitDistance = otherBounds.max.x - center.x;
			normal = { 1.0f, 0.0f };
		}
		if (center.y - otherBounds.min.y < exitDistance)
		{
			exitDistance = center.y - otherBounds.min.y;
			normal = { 0.0f, -1.0f };
		}
		if (otherBounds.max.y - center.y < exitDistance)
		{
			exitDistance = otherBounds.max.y - center.y;
			normal = { 0.0f, 1.0f };
		}
	}
	else
	{
		const Vector2f offset = center - otherBounds.GetCenter();
		const float offsetLength = offset.Magnitude();

		normal = (offsetLength > kCenterInsideEpsilon) ? offset / offsetLength : Vector2f::Up();
		exitDistance = std::max(otherBounds.GetExtents().x - offsetLength, 0.0f);
	}

	const Vector2f pointA = center - normal * m_radius;
	const Vector2f pointB = center + normal * exitDistance;

	return ColliderDistance2D(pointA, pointB, normal, -(exitDistance + m_radius), true);
}
