/*------------------------------
| File: Transform.cpp
| Author: Chandler Mays
------------------------------*/
#include "Transform.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------
| --- Constructor: Constructs the Transform with default values --- |
-------------------------------------------------------------------*/
CE::Transform::Transform()
	: m_position{ 0.0f, 0.0f }
	, m_rotation{ 0.0f }
	, m_scale{ 1.0f, 1.0f }
{}

/*------------------------------------------------------------------------------------------
| --- Parameterized Constructor: Sets initial values for position, rotation, and scale --- |
------------------------------------------------------------------------------------------*/
CE::Transform::Transform(const Vector2f& position, float rotation, const Vector2f& scale)
	: m_position{ position }
	, m_rotation{ rotation }
	, m_scale{ scale }
{}

/*---------------------------------------------------------
| --- GetPosition: Returns the position of the object --- |
---------------------------------------------------------*/
const CE::Vector2f& CE::Transform::GetPosition() const
{
	return m_position;
}

/*------------------------------------------------------
| --- SetPosition: Sets the position of the object --- |
------------------------------------------------------*/
void CE::Transform::SetPosition(const Vector2f& position)
{
	m_position = position;
}

/*---------------------------------------------------------
| --- GetRotation: Returns the rotation of the object --- |
---------------------------------------------------------*/
float CE::Transform::GetRotation() const
{
	return m_rotation;
}

/*------------------------------------------------------
| --- SetRotation: Sets the rotation of the object --- |
------------------------------------------------------*/
void CE::Transform::SetRotation(float rotation)
{
	m_rotation = rotation;
}

/*---------------------------------------------------
| --- GetScale: Returns the scale of the object --- |
---------------------------------------------------*/
const CE::Vector2f& CE::Transform::GetScale() const
{
	return m_scale;
}

/*------------------------------------------------
| --- SetScale: Sets the scale of the object --- |
------------------------------------------------*/
void CE::Transform::SetScale(const Vector2f& scale)
{
	m_scale = scale;
}