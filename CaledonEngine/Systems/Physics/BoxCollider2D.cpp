#include "BoxCollider2D.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the BoxCollider2D with default values --- |
-----------------------------------------------------------------------*/
CE::BoxCollider2D::BoxCollider2D()
    : ColliderComponent()
    , m_width{ 100 }
	, m_height{ 100 }
{
	m_colliderType = ColliderType::kBox2D;
}

/*--------------------------------------------------------
| --- Initialize: Prepares the BoxCollider2D for use --- |
--------------------------------------------------------*/
bool CE::BoxCollider2D::Initialize()
{
    if (!ColliderComponent::Initialize())
        return false;

	m_bounds.m_width = m_width;
	m_bounds.m_height = m_height;

	return m_width > 0 && m_height > 0;
}

/*------------------------------------------------------------------------------------------
| --- ContainsPoint: Returns whether the given point is inside the box collider or not --- |
------------------------------------------------------------------------------------------*/
bool CE::BoxCollider2D::ContainsPoint(const Vector2f& point) const
{
    return point.x >= m_bounds.m_x && point.x <= m_bounds.m_x + m_bounds.m_width &&
        point.y >= m_bounds.m_y && point.y <= m_bounds.m_y + m_bounds.m_height;
}

/*----------------------------------------------------------------------------------------
| --- ClosestPoint: Returns the closest point on the box collider to the given point --- |
----------------------------------------------------------------------------------------*/
CE::Vector2f CE::BoxCollider2D::ClosestPoint(const Vector2f& point) const
{
    Vector2f closestPoint;

    closestPoint.x = std::max(static_cast<float>(m_bounds.m_x), std::min(point.x, static_cast<float>(m_bounds.m_x + m_bounds.m_width)));
    closestPoint.y = std::max(static_cast<float>(m_bounds.m_y), std::min(point.y, static_cast<float>(m_bounds.m_y + m_bounds.m_height)));

    return closestPoint;
}

/*----------------------------------------------------
| --- SetSize: Sets the size of the box collider --- |
----------------------------------------------------*/
void CE::BoxCollider2D::SetSize(int width, int height)
{
    m_width = width;
	m_height = height;

    m_bounds.m_width = m_width;
    m_bounds.m_height = m_height;
}

/*---------------------------------------------------------
| --- GetWidth: Returns the width of the box collider --- |
---------------------------------------------------------*/
int CE::BoxCollider2D::GetWidth() const
{
    return m_width;
}

/*-----------------------------------------------------------
| --- GetHeight: Returns the height of the box collider --- |
-----------------------------------------------------------*/
int CE::BoxCollider2D::GetHeight() const
{
	return m_height;
}