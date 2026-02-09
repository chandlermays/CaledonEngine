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

	//...?
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

	//...?
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
	//...?
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