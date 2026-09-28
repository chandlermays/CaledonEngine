/*------------------------------
| File: CicleCollider2D.cpp
| Author: Chandler Mays
------------------------------*/
#include "CircleCollider2D.h"

#include "Core/GameObject.h"
#include "Core/Transform.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------------
| --- Constructor: Constructs the CircleCollider2D with default values --- |
--------------------------------------------------------------------------*/
CE::CircleCollider2D::CircleCollider2D()
	: Collider2D()
	, m_size{ 1.0f, 1.0f }
	, m_radius{ 1.0f }
{ }

/*------------------------------------------------------------------------------------------------------------------
| --- ClosestPoint: Returns the closest point on the circle collider's surface to a given point in world space --- |
------------------------------------------------------------------------------------------------------------------*/
CE::Vector2f CE::CircleCollider2D::ClosestPoint(const Vector2f& point) const
{
	//...
}

/*---------------------------------------------------------------------------------------
| --- Overlaps: Returns true if this circle collider overlaps with another collider --- |
---------------------------------------------------------------------------------------*/
bool CE::CircleCollider2D::Overlaps(const Collider2D& other) const
{
	//...
}

/*-----------------------------------------------------------------------------------
| --- SetSize: Sets the size of the circle collider and recalculates the bounds --- |
-----------------------------------------------------------------------------------*/
void CE::CircleCollider2D::SetSize(const Vector2f& size)
{
	m_size = size;
}

/*----------------------------------------------------------
| --- GetSize: Returns the size of the circle collider --- |
----------------------------------------------------------*/
const CE::Vector2f& CE::CircleCollider2D::GetSize() const
{
	return m_size;
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

	//...
}