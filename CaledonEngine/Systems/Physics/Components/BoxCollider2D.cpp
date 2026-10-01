/*------------------------------
| File: BoxCollider2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "BoxCollider2D.h"

#include "Core/GameObject.h"
#include "Core/Transform.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the BoxCollider2D with default values --- |
-----------------------------------------------------------------------*/
CE::BoxCollider2D::BoxCollider2D()
	: Collider2D()
	, m_size{ 1.0f, 1.0f }
	, m_edgeRadius{ 1.0f }
{ }

/*------------------------------------------------------------------------------------------------------------------
| --- ClosestPoint: Returns the closest point on the box collider's surface to a given position in world space --- |
------------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::BoxCollider2D::ClosestPoint(const Vector2f& position) const
{
	// Fallback to sharp-cornered math if edge radius is not used
	if (m_edgeRadius <= 0.0f)
		return Vector2f::Clamp(position, m_bounds.min, m_bounds.max);

	const Vector2f center = m_bounds.GetCenter();
	const Vector2f extents = m_bounds.GetExtents();
	const Vector2f offset = position - center;

	// Shrink the outer bounds by the edge radius to define the inner sharp box
	const Vector2f innerExtents = Vector2f::Max(extents - m_edgeRadius, Vector2f::Zero());

	// Find the closest point in/on the inner box relative to the center
	const Vector2f clampedOffset = Vector2f::Clamp(offset, -innerExtents, innerExtents);

	// Calculate the vector from the inner box's surface to the target point
	const Vector2f toPoint = offset - clampedOffset;
	const float sqrDistance = toPoint.SqrMagnitude();

	// If the distance is within the edge radius, the point is inside the solid rounded box
	if (sqrDistance <= m_edgeRadius * m_edgeRadius)
		return position;

	// Otherwise, project the point onto the rounded boundary
	const Vector2f surfaceOffset = clampedOffset + toPoint * (m_edgeRadius / std::sqrt(sqrDistance));

	return center + surfaceOffset;
}

/*------------------------------------------------------------------------------------
| --- Overlaps: Returns true if this box collider overlaps with another collider --- |
------------------------------------------------------------------------------------*/
bool CE::BoxCollider2D::Overlaps(const Collider2D& other) const
{
	if (!other.IsAxisAlignedBox())
		return other.Overlaps(*this);

	return m_bounds.Overlaps(other.GetBounds());
}

/*--------------------------------------------------------------------------------
| --- SetSize: Sets the size of the box collider and recalculates the bounds --- |
--------------------------------------------------------------------------------*/
void CE::BoxCollider2D::SetSize(const Vector2f& size)
{
	m_size = size;
	RecalculateBounds();
}

/*-------------------------------------------------------
| --- GetSize: Returns the size of the box collider --- |
-------------------------------------------------------*/
const CE::Vector2f& CE::BoxCollider2D::GetSize() const
{
	return m_size;
}

/*---------------------------------------------------------------------------------
| --- SetEdgeRadius: Sets the radius of the rounded edges of the box collider --- |
---------------------------------------------------------------------------------*/
void CE::BoxCollider2D::SetEdgeRadius(float radius)
{
	m_edgeRadius = radius;
}

/*------------------------------------------------------------------------------------
| --- GetEdgeRadius: Returns the radius of the rounded edges of the box collider --- |
------------------------------------------------------------------------------------*/
float CE::BoxCollider2D::GetEdgeRadius() const
{
	return m_edgeRadius;
}

/*-------------------------------------------------------------
| --- GetTypeName: Returns the type name of the Component --- |
-------------------------------------------------------------*/
const std::string& CE::BoxCollider2D::GetTypeName() const
{
	static const std::string typeName = "BoxCollider2D";
	return typeName;
}



/*--------------------------------------
| --- Protected Method Definitions --- |
--------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------
| --- RecalculateBounds: Recalculates the AABB based on the GameObject's transform and the box collider's properties --- |
------------------------------------------------------------------------------------------------------------------------*/
void CE::BoxCollider2D::RecalculateBounds()
{
	if (!m_pOwner)
		return;

	Vector2f worldPosition = m_pOwner->GetTransform().GetPosition() + m_offset;
	Vector2f halfSize = m_size / 2.0f;

	m_bounds.min = worldPosition - halfSize;
	m_bounds.max = worldPosition + halfSize;
}