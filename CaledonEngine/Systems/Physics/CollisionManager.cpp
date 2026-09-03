/*------------------------------
| File: CollisionManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "CollisionManager.h"
#include "Core/GameObject.h"
#include "Core/Transform.h"
#include "Systems/Engine/LoggingManager.h"

#include <algorithm>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::CollisionManager::~CollisionManager()
{
	CE_LOG("EngineManager::~EngineManager - Shutting down EngineManager.");
	Shutdown();
}
/*-----------------------------------------------------------
| --- Initialize: Prepares the CollisionManager for use --- |
-----------------------------------------------------------*/
bool CE::CollisionManager::Initialize()
{
	return true;
}

/*--------------------------------------------------------------------------------------------
| --- Update: Updates the collision system, checking for overlaps and invoking callbacks --- |
--------------------------------------------------------------------------------------------*/
void CE::CollisionManager::Update(float)
{
	m_currentOverlaps.clear();

	for (size_t i = 0; i < m_activeColliders.size(); ++i)
	{
		Collider2D* pA = m_activeColliders[i];
		if (!pA || !pA->IsActive() || !pA->GetOwner())
			continue;

		for (size_t j = i + 1; j < m_activeColliders.size(); ++j)
		{
			Collider2D* pB = m_activeColliders[j];
			if (!pB || !pB->IsActive() || !pB->GetOwner())
				continue;

			// Broad-phase: cheap AABB test first
			if (!pA->GetBounds().Overlaps(pB->GetBounds()))
				continue;

			// Narrow-phase: exact shape test — identical to broad-phase for two boxes today
			// (a box's true shape IS its AABB), starts to matter once CircleCollider2D exists.
			if (!pA->Overlaps(*pB))
				continue;

			ColliderPair pair = MakePair(pA, pB);
			m_currentOverlaps.insert(pair);

			bool wasOverlapping = m_previousOverlaps.count(pair) > 0;
			bool isTriggerPair = pA->IsTrigger() || pB->IsTrigger();

			if (wasOverlapping)
			{
				pA->InvokeUpdate(pB);
				pB->InvokeUpdate(pA);
			}
			else
			{
				pA->InvokeEnter(pB);
				pB->InvokeEnter(pA);
			}

			// Any pair involving a trigger is overlap-only; only solid-vs-solid gets pushed apart.
			if (!isTriggerPair)
			{
				ResolveCollision(pA, pB);
			}
		}
	}

	for (const ColliderPair& pair : m_previousOverlaps)
	{
		if (m_currentOverlaps.find(pair) == m_currentOverlaps.end())
		{
			pair.first->InvokeExit(pair.second);
			pair.second->InvokeExit(pair.first);
		}
	}

	m_previousOverlaps = std::move(m_currentOverlaps);
}

/*-----------------------------------------------------------------
| --- Shutdown: Cleans up and shuts down the collision system --- |
-----------------------------------------------------------------*/
void CE::CollisionManager::Shutdown()
{
	m_activeColliders.clear();
	m_currentOverlaps.clear();
	m_previousOverlaps.clear();
}

/*----------------------------------------------------------------------------
| --- AddActiveCollider: Adds a collider to the list of active colliders --- |
----------------------------------------------------------------------------*/
void CE::CollisionManager::AddActiveCollider(Collider2D* pCollider)
{
	if (pCollider && std::find(m_activeColliders.begin(), m_activeColliders.end(), pCollider) == m_activeColliders.end())
	{
		m_activeColliders.push_back(pCollider);
	}
}

/*------------------------------------------------------------------------------------
| --- RemoveActiveCollider: Removes a collider from the list of active colliders --- |
------------------------------------------------------------------------------------*/
void CE::CollisionManager::RemoveActiveCollider(Collider2D* pCollider)
{
	auto it = std::find(m_activeColliders.begin(), m_activeColliders.end(), pCollider);
	if (it != m_activeColliders.end())
	{
		m_activeColliders.erase(it);
	}

	// Purge any tracked overlap state involving this collider — otherwise a destroyed
	// collider could trigger a phantom OnCollisionExit next frame against something
	// that no longer exists.
	auto involvesCollider = [pCollider](const ColliderPair& pair)
		{
			return pair.first == pCollider || pair.second == pCollider;
		};

	for (auto it2 = m_currentOverlaps.begin(); it2 != m_currentOverlaps.end(); )
	{
		it2 = involvesCollider(*it2) ? m_currentOverlaps.erase(it2) : std::next(it2);
	}
	for (auto it2 = m_previousOverlaps.begin(); it2 != m_previousOverlaps.end(); )
	{
		it2 = involvesCollider(*it2) ? m_previousOverlaps.erase(it2) : std::next(it2);
	}
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*---------------------------------------------------------------------------------------------------------
| --- MakePair: Creates a sorted pair of colliders to ensure consistent ordering for overlap tracking --- |
---------------------------------------------------------------------------------------------------------*/
CE::CollisionManager::ColliderPair CE::CollisionManager::MakePair(Collider2D* pA, Collider2D* pB)
{
	return (pA < pB) ? ColliderPair(pA, pB) : ColliderPair(pB, pA);
}

/*---------------------------------------------------------------------------------------------------------------
| --- ResolveCollision: Resolves the collision between two colliders, applying a simple separation response --- |
---------------------------------------------------------------------------------------------------------------*/
void CE::CollisionManager::ResolveCollision(Collider2D* pA, Collider2D* pB)
{
	// Minimum-translation-vector separation along whichever axis has less overlap.
	// Honest simplification worth naming: with no Rigidbody/velocity/static-vs-dynamic
	// concept in the engine yet, this just splits the correction evenly between both
	// objects — a working first pass, not a final physics model.
	const AABB2D& boundsA = pA->GetBounds();
	const AABB2D& boundsB = pB->GetBounds();

	float overlapX = std::min(boundsA.max.x, boundsB.max.x) - std::max(boundsA.min.x, boundsB.min.x);
	float overlapY = std::min(boundsA.max.y, boundsB.max.y) - std::max(boundsA.min.y, boundsB.min.y);

	if (overlapX <= 0.0f || overlapY <= 0.0f)
		return;

	Vector2f centerA = boundsA.GetCenter();
	Vector2f centerB = boundsB.GetCenter();
	Vector2f pushDirection;
	float pushAmount;

	if (overlapX < overlapY)
	{
		pushAmount = overlapX * 0.5f;
		pushDirection = { (centerA.x < centerB.x) ? -1.0f : 1.0f, 0.0f };
	}
	else
	{
		pushAmount = overlapY * 0.5f;
		pushDirection = { 0.0f, (centerA.y < centerB.y) ? -1.0f : 1.0f };
	}

	Transform& transformA = pA->GetOwner()->GetTransform();
	Transform& transformB = pB->GetOwner()->GetTransform();

	transformA.SetPosition(transformA.GetPosition() + pushDirection * pushAmount);
	transformB.SetPosition(transformB.GetPosition() - pushDirection * pushAmount);
}