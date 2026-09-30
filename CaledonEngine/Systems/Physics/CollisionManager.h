/*------------------------------
| File: CollisionManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Physics/Components/Collider2D.h"
#include "Systems/Physics/Types/Contact2D.h"

#include <vector>
#include <set>
#include <utility>

// A pure geometric solver: tracks colliders, finds overlaps, and reports contacts (normal + depth).
// It knows nothing about mass, velocity, or body types, and it never moves anything.
// It is no longer an IUpdatable; PhysicsManager drives it from its fixed step.

namespace CE
{
	class CollisionManager : public Manager
	{
	private:
		using ColliderPair = std::pair<Collider2D*, Collider2D*>;								// Type alias for a pair of Collider2D pointers representing a collision pair

		std::vector<Collider2D*> m_activeColliders;												// List of all active colliders in the scene
		std::set<ColliderPair> m_currentOverlaps;												// Collider pairs overlapping in the dispatch currently being processed
		std::set<ColliderPair> m_previousOverlaps;												// Collider pairs that were overlapping in the previous dispatch

		static ColliderPair MakePair(Collider2D* pA, Collider2D* pB);							// Creates a sorted pair of colliders to ensure consistent ordering for overlap tracking
		static bool TryBuildContact(Collider2D* pA, Collider2D* pB, Contact2D& out);			// Broad-phase + narrow-phase test; builds the contact if the pair overlaps
		static bool BuildContact(Collider2D* pA, Collider2D* pB, Contact2D& out);				// Builds the contact (normal from B to A, depth) for an overlapping pair

	public:
		CollisionManager() = default;															// Constructor
		~CollisionManager() override;															// Destructor
		CollisionManager(const CollisionManager&) = delete;										// Prevent copy-construction
		CollisionManager& operator=(const CollisionManager&) = delete;							// Prevent copy-assignment
		CollisionManager(CollisionManager&&) = delete;											// Prevent move-construction
		CollisionManager& operator=(CollisionManager&&) = delete;								// Prevent move-assignment

		bool Initialize() override;																// Prepares the CollisionManager for use
		void Shutdown() override;																// Cleans up and shuts down the collision system

		void RefreshAllBounds() const;															// Recalculates every active collider's bounds from its GameObject's current Transform
		std::vector<Contact2D> QueryContacts(Collider2D* pCollider) const;						// Contacts of ONE collider against all others (normal points toward pCollider); does not refresh bounds
		std::vector<Contact2D> DetectContacts() const;											// Refreshes collider bounds, then returns every overlapping pair as a contact (triggers included, flagged)
		void DispatchContactEvents(const std::vector<Contact2D>& contacts);						// Invokes Enter / Update / Exit callbacks by comparing these contacts to the previous dispatch

		void AddActiveCollider(Collider2D* pCollider);											// Adds a collider to the list of active colliders
		void RemoveActiveCollider(Collider2D* pCollider);										// Removes a collider from the list of active colliders
	};
}