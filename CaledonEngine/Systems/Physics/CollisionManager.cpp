#include "CollisionManager.h"
#include "ColliderComponent.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Utilities/Math/Rect.h"

/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*--------------------------------------------------------------------
| --- CheckCollision: Checks for collision between two colliders --- |
--------------------------------------------------------------------*/
bool CE::CollisionManager::CheckCollision(ColliderComponent* pColliderA, ColliderComponent* pColliderB)
{
	if (!pColliderA || !pColliderB)
		return false;

	// For now, we will only implement AABB collision detection for BoxCollider2D types
	return AABBIntersect(pColliderA->GetBounds(), pColliderB->GetBounds());
}

/*----------------------------------------------------------------------------------------
| --- AABBIntersect: Checks for intersection between two axis-aligned bounding boxes --- |
----------------------------------------------------------------------------------------*/
bool CE::CollisionManager::AABBIntersect(const Rect& a, const Rect& b)
{
	return (a.m_x < b.m_x + b.m_width && a.m_x + a.m_width > b.m_x && a.m_y < b.m_y + b.m_height && a.m_y + a.m_height > b.m_y);
}

/*----------------------------------------------------------------------------------------------------------------------------------
| --- HandleCollision: Handles the collision events between two colliders based on their current and previous collision states --- |
----------------------------------------------------------------------------------------------------------------------------------*/
void CE::CollisionManager::HandleCollision(ColliderComponent* pColliderA, ColliderComponent* pColliderB, bool isColliding)
{
	if (!pColliderA || !pColliderB)
		return;

	// Normalize pair order for consistent lookup
	ColliderComponent* pFirst = pColliderA;
	ColliderComponent* pSecond = pColliderB;
	if (pFirst > pSecond)
		std::swap(pFirst, pSecond);

	// Check if this pair was colliding in the previous frame
	auto it = std::find(m_previousCollisions.begin(), m_previousCollisions.end(), 
					   std::make_pair(pFirst, pSecond));
	bool wasColliding = it != m_previousCollisions.end();

	if (isColliding)
	{
		if (!wasColliding)
		{
			// Collision just started
			CollisionInfo infoA{};
			infoA.m_pOtherCollider = pColliderB;
			infoA.m_pOtherObject = pColliderB->GetOwner();

			CollisionInfo infoB{};
			infoB.m_pOtherCollider = pColliderA;
			infoB.m_pOtherObject = pColliderA->GetOwner();

			if (pColliderA->IsTrigger() || pColliderB->IsTrigger())
			{
				pColliderA->OnTriggerEnter(infoA);
				pColliderB->OnTriggerEnter(infoB);
			}
			else
			{
				pColliderA->OnCollisionEnter(infoA);
				pColliderB->OnCollisionEnter(infoB);
			}

			// Add to previous collisions list
			m_previousCollisions.emplace_back(pFirst, pSecond);
		}
		else
		{
			// Collision is ongoing
			CollisionInfo infoA{};
			infoA.m_pOtherCollider = pColliderB;
			infoA.m_pOtherObject = pColliderB->GetOwner();

			CollisionInfo infoB{};
			infoB.m_pOtherCollider = pColliderA;
			infoB.m_pOtherObject = pColliderA->GetOwner();

			if (pColliderA->IsTrigger() || pColliderB->IsTrigger())
			{
				pColliderA->OnTriggerUpdate(infoA);
				pColliderB->OnTriggerUpdate(infoB);
			}
			else
			{
				pColliderA->OnCollisionUpdate(infoA);
				pColliderB->OnCollisionUpdate(infoB);
			}
		}
	}
	else
	{
		if (wasColliding)
		{
			// Collision has ended
			CollisionInfo infoA{};
			infoA.m_pOtherCollider = pColliderB;
			infoA.m_pOtherObject = pColliderB->GetOwner();

			CollisionInfo infoB{};
			infoB.m_pOtherCollider = pColliderA;
			infoB.m_pOtherObject = pColliderA->GetOwner();

			if (pColliderA->IsTrigger() || pColliderB->IsTrigger())
			{
				pColliderA->OnTriggerExit(infoA);
				pColliderB->OnTriggerExit(infoB);
			}
			else
			{
				pColliderA->OnCollisionExit(infoA);
				pColliderB->OnCollisionExit(infoB);
			}

			// Remove from previous collisions list
			m_previousCollisions.erase(it);
		}
	}
}


/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*---------------------------------------------------------------------------
| --- Constructor: Constructs the Collision Manager with default values --- |
---------------------------------------------------------------------------*/
CE::CollisionManager::CollisionManager()
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::CollisionManager::~CollisionManager()
{
	CE_LOG("CollisionManager::~CollisionManager - Shutting down CollisionManager.");
	Shutdown();
}

/*------------------------------------------------------------
| --- Initialize: Prepares the Collision Manager for use --- |
------------------------------------------------------------*/
bool CE::CollisionManager::Initialize()
{
	m_colliders.clear();
	m_previousCollisions.clear();
	return true;
}

/*------------------------------------------------------------------------------
| --- Update: Updates the collision detection for all registered colliders --- |
------------------------------------------------------------------------------*/
void CE::CollisionManager::Update(float deltaTime)
{
	std::vector<std::pair<ColliderComponent*, ColliderComponent*>> currentCollisions;

	// Check for collisions between all pairs of colliders
	for (size_t i = 0; i < m_colliders.size(); ++i)
	{
		for (size_t j = i + 1; j < m_colliders.size(); ++j)
		{
			ColliderComponent* pColliderA = m_colliders[i];
			ColliderComponent* pColliderB = m_colliders[j];

			// skip if either collider is null or if either collider's owner is null
			if (!pColliderA || !pColliderB || !pColliderA->GetOwner() || !pColliderB->GetOwner())
				continue;

			bool isColliding = CheckCollision(pColliderA, pColliderB);

			if (isColliding)
			{
				auto pair = (pColliderA < pColliderB) ? std::make_pair(pColliderA, pColliderB) : std::make_pair(pColliderB, pColliderA);
				currentCollisions.emplace_back(pair);
			}

			HandleCollision(pColliderA, pColliderB, isColliding);
		}
	}

	// Update the previous collisions list for the next frame
	m_previousCollisions = currentCollisions;
}

/*------------------------------------------------------------------
| --- Shutdown: Cleans up and shuts down the collision manager --- |
------------------------------------------------------------------*/
void CE::CollisionManager::Shutdown()
{
	m_colliders.clear();
	m_previousCollisions.clear();
}

/*---------------------------------------------------------------------------
| --- RegisterCollider: Registers a collider with the collision manager --- |
---------------------------------------------------------------------------*/
void CE::CollisionManager::RegisterCollider(ColliderComponent* pCollider)
{
	if (!pCollider)
		return;

	auto it = std::find(m_colliders.begin(), m_colliders.end(), pCollider);
	if (it == m_colliders.end())
	{
		m_colliders.emplace_back(pCollider);
	}
}

/*-------------------------------------------------------------------------------
| --- UnregisterCollider: Unregisters a collider from the collision manager --- |
-------------------------------------------------------------------------------*/
void CE::CollisionManager::UnregisterCollider(ColliderComponent* pCollider)
{
	if (!pCollider)
		return;

	auto it = std::find(m_colliders.begin(), m_colliders.end(), pCollider);
	if (it != m_colliders.end())
	{
		m_colliders.erase(it);
	}
}