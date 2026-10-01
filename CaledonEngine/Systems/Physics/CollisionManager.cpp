/*------------------------------
| File: CollisionManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "CollisionManager.h"

#include "Core/GameObject.h"

#include <algorithm>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::CollisionManager::~CollisionManager()
{
	Shutdown();
}

/*-----------------------------------------------------------
| --- Initialize: Prepares the CollisionManager for use --- |
-----------------------------------------------------------*/
bool CE::CollisionManager::Initialize()
{
	return true;
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

/*--------------------------------------------------------------------------------------------------
| --- RefreshAllBounds: Recalculates every active collider's bounds from its current Transform --- |
--------------------------------------------------------------------------------------------------*/
void CE::CollisionManager::RefreshAllBounds() const
{
	for (Collider2D* pCollider : m_activeColliders)
	{
		if (pCollider && pCollider->GetOwner())
		{
			pCollider->RefreshBounds();
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------------------
| --- QueryContacts: Contacts of ONE collider against all others (normal points toward pCollider); does not refresh bounds --- |
------------------------------------------------------------------------------------------------------------------------------*/
std::vector<CE::Contact2D> CE::CollisionManager::QueryContacts(Collider2D* pCollider) const
{
	std::vector<Contact2D> contacts;

	if (!pCollider || !pCollider->IsActive() || !pCollider->GetOwner())
		return contacts;

	for (Collider2D* pOther : m_activeColliders)
	{
		if (!pOther || pOther == pCollider || !pOther->IsActive() || !pOther->GetOwner())
			continue;

		Contact2D contact;
		if (TryBuildContact(pCollider, pOther, contact))
		{
			contacts.push_back(contact);
		}
	}

	return contacts;
}

/*-----------------------------------------------------------------------------------------------------
| --- DetectContacts: Refreshes bounds, then returns every overlapping collider pair as a contact --- |
-----------------------------------------------------------------------------------------------------*/
std::vector<CE::Contact2D> CE::CollisionManager::DetectContacts() const
{
	// Bounds follow Transforms, and Transforms move during the physics step (and in gameplay Update),
	// so refresh first; otherwise contacts would be computed from last frame's positions.
	RefreshAllBounds();

	std::vector<Contact2D> contacts;

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

			Contact2D contact;
			if (TryBuildContact(pA, pB, contact))
			{
				contacts.push_back(contact);
			}
		}
	}

	return contacts;
}

/*----------------------------------------------------------------------------------------------------
| --- DispatchContactEvents: Invokes Enter / Update / Exit callbacks from the given contact list --- |
----------------------------------------------------------------------------------------------------*/
void CE::CollisionManager::DispatchContactEvents(const std::vector<Contact2D>& contacts)
{
	m_currentOverlaps.clear();

	for (const Contact2D& contact : contacts)
	{
		Collider2D* pA = contact.pColliderA;
		Collider2D* pB = contact.pColliderB;

		ColliderPair pair = MakePair(pA, pB);
		m_currentOverlaps.insert(pair);

		if (m_previousOverlaps.count(pair) > 0)
		{
			pA->InvokeUpdate(pB);
			pB->InvokeUpdate(pA);
		}
		else
		{
			pA->InvokeEnter(pB);
			pB->InvokeEnter(pA);
		}
	}

	// Collect exits first: callbacks can destroy colliders, which purges the overlap sets mid-iteration
	std::vector<ColliderPair> exited;
	for (const ColliderPair& pair : m_previousOverlaps)
	{
		if (m_currentOverlaps.find(pair) == m_currentOverlaps.end())
		{
			exited.push_back(pair);
		}
	}

	for (const ColliderPair& pair : exited)
	{
		pair.first->InvokeExit(pair.second);
		pair.second->InvokeExit(pair.first);
	}

	m_previousOverlaps = std::move(m_currentOverlaps);
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

	// Purge any tracked overlap state involving this collider so a destroyed collider
	// can't trigger a phantom OnCollisionExit against something that no longer exists.
	auto involvesCollider = [pCollider](const ColliderPair& pair)
		{
			return pair.first == pCollider || pair.second == pCollider;
		};

	std::erase_if(m_currentOverlaps, involvesCollider);
	std::erase_if(m_previousOverlaps, involvesCollider);
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

/*-------------------------------------------------------------------------------------------------------
| --- TryBuildContact: Broad-phase AABB test, then narrow-phase shape test, then builds the contact --- |
-------------------------------------------------------------------------------------------------------*/
bool CE::CollisionManager::TryBuildContact(Collider2D* pA, Collider2D* pB, Contact2D& out)
{
	if (!pA->GetBounds().Overlaps(pB->GetBounds()))
		return false;

	if (!pA->Overlaps(*pB))
		return false;

	return BuildContact(pA, pB, out);
}

/*----------------------------------------------------------------------------------------------
| --- BuildContact: Builds the contact (normal from B to A, depth) for an overlapping pair --- |
----------------------------------------------------------------------------------------------*/
bool CE::CollisionManager::BuildContact(Collider2D* pA, Collider2D* pB, Contact2D& out)
{
	const AABB2D& boundsA = pA->GetBounds();
	const AABB2D& boundsB = pB->GetBounds();

	const float overlapX = std::min(boundsA.max.x, boundsB.max.x) - std::max(boundsA.min.x, boundsB.min.x);
	const float overlapY = std::min(boundsA.max.y, boundsB.max.y) - std::max(boundsA.min.y, boundsB.min.y);

	if (overlapX < 0.0f || overlapY < 0.0f)
		return false;

	const Vector2f centerA = boundsA.GetCenter();
	const Vector2f centerB = boundsB.GetCenter();

	Vector2f normal;
	float depth;

	if (overlapX < overlapY)
	{
		depth = overlapX;
		normal = { (centerA.x < centerB.x) ? -1.0f : 1.0f, 0.0f };
	}
	else
	{
		depth = overlapY;
		normal = { 0.0f, (centerA.y < centerB.y) ? -1.0f : 1.0f };
	}

	out = Contact2D(pA, pB, normal, depth, pA->IsTrigger() || pB->IsTrigger());
	return true;
}