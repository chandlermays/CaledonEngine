/*------------------------------
| File: CollisionManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Engine/IUpdatable.h"
#include "Systems/Physics/Components/Collider2D.h"

#include <vector>
#include <set>
#include <utility>

namespace CE
{
	class CollisionManager : public Manager, public IUpdatable
	{
	private:
		using ColliderPair = std::pair<Collider2D*, Collider2D*>;					// Type alias for a pair of Collider2D pointers representing a collision pair

		std::vector<Collider2D*> m_activeColliders;									// List of all active colliders in the scene
		std::set<ColliderPair> m_currentOverlaps;									// Set of collider pairs that are currently overlapping in this frame
		std::set<ColliderPair> m_previousOverlaps;									// Set of collider pairs that were overlapping in the previous frame

		static ColliderPair MakePair(Collider2D* pA, Collider2D* pB);				// Creates a sorted pair of colliders to ensure consistent ordering for overlap tracking
		void ResolveCollision(Collider2D* pA, Collider2D* pB);						// Resolves the collision between two colliders, applying a simple separation response

	public:
		CollisionManager() = default;												// Constructor
		~CollisionManager() override;												// Destructor
		CollisionManager(const CollisionManager&) = delete;							// Prevent copy-construction
		CollisionManager& operator=(const CollisionManager&) = delete;				// Prevent copy-assignment
		CollisionManager(CollisionManager&&) = delete;								// Prevent move-construction
		CollisionManager& operator=(CollisionManager&&) = delete;					// Prevent move-assignment

		bool Initialize() override;													// Prepares the CollisionManager for use
		void Update(float deltaTime) override;										// Updates the collision system, checking for overlaps and invoking callbacks
		void Shutdown() override;													// Cleans up and shuts down the collision system

		void AddActiveCollider(Collider2D* pCollider);								// Adds a collider to the list of active colliders
		void RemoveActiveCollider(Collider2D* pCollider);							// Removes a collider from the list of active colliders
	};
}