#include "ColliderComponent.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"

/*--------------------------------------
| --- Protected Method Definitions --- |
--------------------------------------*/
/*---------------------------------------------------------------------------
| --- Constructor: Constructs the ColliderComponent with default values --- |
---------------------------------------------------------------------------*/
CE::ColliderComponent::ColliderComponent()
	: Component()
    , m_colliderType{ }
    , m_bounds{ 0, 0, 0, 0 }
	, m_offset{ 0.0f, 0.0f }
	, m_isTrigger{ false }
{ }


/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::ColliderComponent::~ColliderComponent()
{
	m_overlappingColliders.clear();
}

/*------------------------------------------------------------
| --- Initialize: Prepares the ColliderComponent for use --- |
------------------------------------------------------------*/
bool CE::ColliderComponent::Initialize()
{
    if (!m_pOwner)
        return false;

    UpdateBounds();

	return true;
}

/*-----------------------------------------------------------------------
| --- Update: Updates the collider bounds based on owner's position --- |
-----------------------------------------------------------------------*/
void CE::ColliderComponent::Update(float deltaTime)
{
	UpdateBounds();
}

/*-----------------------------------------------------------------------------
| --- Overlap: Returns a list of all colliders that overlap this collider --- |
-----------------------------------------------------------------------------*/
std::vector<CE::ColliderComponent*> CE::ColliderComponent::Overlap() const
{
	return m_overlappingColliders;
}

/*-----------------------------------------------------------------------------------
| --- IsTouching: Returns whether this collider is touching the collider or not --- |
-----------------------------------------------------------------------------------*/
bool CE::ColliderComponent::IsTouching(const ColliderComponent* pOther) const
{
    if (!pOther)
        return false;

	// check if the other collider is in the overlapping colliders list
    auto it = std::find(m_overlappingColliders.begin(), m_overlappingColliders.end(), pOther);
	return it != m_overlappingColliders.end();
}

/*-----------------------------------------------------------------------
| --- SetBounds: Sets the world space bounding area of the collider --- |
-----------------------------------------------------------------------*/
void CE::ColliderComponent::SetBounds(const Rect& bounds)
{
	m_bounds = bounds;
}

/*-------------------------------------------------------------------
| --- SetOffset: Sets the local offset of the collider geometry --- |
-------------------------------------------------------------------*/
void CE::ColliderComponent::SetOffset(const Vector2f& offset)
{
	m_offset = offset;

    UpdateBounds();
}

/*-------------------------------------------------------------------
| --- SetTrigger: Sets whether the collider is a trigger or not --- |
-------------------------------------------------------------------*/
void CE::ColliderComponent::SetTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;
}

/*-------------------------------------------------------------------------------------------------------
| --- UpdateBounds: Updates the world space bounding area of the collider based on owner's position --- |
-------------------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::UpdateBounds()
{
    if (!m_pOwner)
        return;

    const Vector2f& position = m_pOwner->GetTransform().GetPosition();

	m_bounds.m_x = static_cast<int>(position.x + m_offset.x);
	m_bounds.m_y = static_cast<int>(position.y + m_offset.y);
}

/*-----------------------------------------------------------
| --- GetColliderType: Returns the type of the collider --- |
-----------------------------------------------------------*/
CE::ColliderType CE::ColliderComponent::GetColliderType() const
{
	return m_colliderType;
}

/*--------------------------------------------------------------------------
| --- GetBounds: Returns the world space bounding area of the collider --- |
--------------------------------------------------------------------------*/
CE::Rect CE::ColliderComponent::GetBounds() const
{
	return m_bounds;
}

/*----------------------------------------------------------------------
| --- GetOffset: Returns the local offset of the collider geometry --- |
----------------------------------------------------------------------*/
CE::Vector2f CE::ColliderComponent::GetOffset() const
{
	return m_offset;
}

/*---------------------------------------------------------------------
| --- IsTrigger: Returns whether the collider is a trigger or not --- |
---------------------------------------------------------------------*/
bool CE::ColliderComponent::IsTrigger() const
{
	return m_isTrigger;
}

/*--------------------------------------------------------------------------------------------
| --- SetOnCollisionEnterCallback: Sets the callback function for collision enter events --- |
--------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::SetOnCollisionEnterCallback(CollisionCallback callback)
{
	m_onCollisionEnter = callback;
}

/*----------------------------------------------------------------------------------------------
| --- SetOnCollisionUpdateCallback: Sets the callback function for collision update events --- |
----------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::SetOnCollisionUpdateCallback(CollisionCallback callback)
{
	m_onCollisionUpdate = callback;
}

/*------------------------------------------------------------------------------------------
| --- SetOnCollisionExitCallback: Sets the callback function for collision exit events --- |
------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::SetOnCollisionExitCallback(CollisionCallback callback)
{
	m_onCollisionExit = callback;
}

/*----------------------------------------------------------------------------------------
| --- SetOnTriggerEnterCallback: Sets the callback function for trigger enter events --- |
----------------------------------------------------------------------------------------*/
void CE::ColliderComponent::SetOnTriggerEnterCallback(CollisionCallback callback)
{
	m_onTriggerEnter = callback;
}

/*------------------------------------------------------------------------------------------
| --- SetOnTriggerUpdateCallback: Sets the callback function for trigger update events --- |
------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::SetOnTriggerUpdateCallback(CollisionCallback callback)
{
	m_onTriggerUpdate = callback;
}

/*--------------------------------------------------------------------------------------
| --- SetOnTriggerExitCallback: Sets the callback function for trigger exit events --- |
--------------------------------------------------------------------------------------*/
void CE::ColliderComponent::SetOnTriggerExitCallback(CollisionCallback callback)
{
	m_onTriggerExit = callback;
}

/*---------------------------------------------------------------
| --- DebugDraw: Renders debug information for the collider --- |
---------------------------------------------------------------*/
void CE::ColliderComponent::DebugDraw() const
{
    // Render the collider's bounds
}