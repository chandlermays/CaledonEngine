/*------------------------------
| File: BoxCollider2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "BoxCollider2D.h"
#include "Core/GameObject.h"
#include "Core/Transform.h"

/*-----------------------------------------------------------------------
| --- Constructor: Constructs the BoxCollider2D with default values --- |
-----------------------------------------------------------------------*/
CE::BoxCollider2D::BoxCollider2D()
	: Collider2D()
	, m_size{ 1.0f, 1.0f }
	, m_edgeRadius{ 0.0f }
{ }

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

/*---------------------------------------------------------------------------------------------------------------
| --- ClosestPoint: Returns the closest point on the box collider's surface to a given point in world space --- |
---------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::BoxCollider2D::ClosestPoint(const Vector2f& point) const
{
	// Treats the box as sharp-cornered — doesn't yet account for edgeRadius rounding
	// the corners inward. Correct rounded-box closest-point math needs a corner-region
	// check; flagging as a known simplification rather than rushing it now.
	return Vector2f::Clamp(point, m_bounds.min, m_bounds.max);
}

/*------------------------------------------------------------------------------------
| --- Overlaps: Returns true if this box collider overlaps with another collider --- |
------------------------------------------------------------------------------------*/
bool CE::BoxCollider2D::Overlaps(const Collider2D& other) const
{
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