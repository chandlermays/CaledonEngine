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

    const Transform& transform = m_pOwner->GetTransform();
	const VectorFloat& position = transform.GetPosition();

    m_bounds.m_x = static_cast<int>(position.m_x + m_offset.m_x);
    m_bounds.m_y = static_cast<int>(position.m_y + m_offset.m_y);

	return true;
}

/*-----------------------------------------------------------------------------------------
| --- OnCollisionEnter: Called when this collider has begun touching another collider --- |
-----------------------------------------------------------------------------------------*/
void CE::ColliderComponent::OnCollisionEnter(const CollisionInfo& info)
{
    // Add to the container of overlapping colliders if not already present

    // Invoke the callback
}

/*----------------------------------------------------------------------------------------------------
| --- OnCollisionUpdate: Called once per frame for every collider that is touching this collider --- |
----------------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::OnCollisionUpdate(const CollisionInfo& info)
{
    // Invoke the callback
}

/*------------------------------------------------------------------------------------------
| --- OnCollisionExit: Called when this collider has stopped touching another collider --- |
------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::OnCollisionExit(const CollisionInfo& info)
{
    // Remove from the container of overlappoing colliders

	// Invoke the callback
}

/*------------------------------------------------------------------------------------------------------
| --- OnTriggerEnter: Called when a collider with the isTrigger property overlaps another collider --- |
------------------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::OnTriggerEnter(const CollisionInfo& info)
{
    // Add to the container of overlapping colliders if not already present

    // Invoke the callback
}

/*------------------------------------------------------------------------------------------------
| --- OnTriggerUpdate: Called once per frame for every collider that is touching the trigger --- |
------------------------------------------------------------------------------------------------*/
void CE::ColliderComponent::OnTriggerUpdate(const CollisionInfo& info)
{
	// Invoke the callback
}

/*----------------------------------------------------------------------------------------
| --- OnTriggerExit: Called when the other collider has stopped touching the trigger --- |
----------------------------------------------------------------------------------------*/
void CE::ColliderComponent::OnTriggerExit(const CollisionInfo& info)
{
    // Remove from the container of overlappoing colliders
    // 
	// Invoke the callback
}

/*-----------------------------------------------------------------------------
| --- Overlap: Returns a list of all colliders that overlap this collider --- |
-----------------------------------------------------------------------------*/
std::vector<CE::ColliderComponent*> CE::ColliderComponent::Overlap() const
{
    return std::vector<ColliderComponent*>();
}

/*-----------------------------------------------------------------------------------
| --- IsTouching: Returns whether this collider is touching the collider or not --- |
-----------------------------------------------------------------------------------*/
bool CE::ColliderComponent::IsTouching(const ColliderComponent* pOther) const
{
    if (!pOther)
        return false;

	// check if the other collider is in the overlapping colliders list
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
void CE::ColliderComponent::SetOffset(const VectorFloat& offset)
{
	m_offset = offset;

    if (m_pOwner != nullptr)
    {
		const Transform& transform = m_pOwner->GetTransform();
        const VectorFloat& position = transform.GetPosition();

        m_bounds.m_x = static_cast<int>(position.m_x + m_offset.m_x);
		m_bounds.m_y = static_cast<int>(position.m_y + m_offset.m_y);
    }
}

/*-------------------------------------------------------------------
| --- SetTrigger: Sets whether the collider is a trigger or not --- |
-------------------------------------------------------------------*/
void CE::ColliderComponent::SetTrigger(bool isTrigger)
{
	m_isTrigger = isTrigger;
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
CE::VectorFloat CE::ColliderComponent::GetOffset() const
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

/*---------------------------------------------------------------
| --- DebugDraw: Renders debug information for the collider --- |
---------------------------------------------------------------*/
void CE::ColliderComponent::DebugDraw() const
{
    // Render the collider's bounds
}