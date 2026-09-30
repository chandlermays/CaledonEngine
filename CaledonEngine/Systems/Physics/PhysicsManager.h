/*------------------------------
| File: PhysicsManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Physics/Types/Contact2D.h"
#include "Utilities/Math/Vector2.h"

#include <unordered_map>
#include <vector>

// The orchestrator between forces and geometry.
//
// PhysicsManager owns gravity and the list of RigidBody2D, and is stepped by EngineManager's fixed-timestep
// accumulator (it is NOT an IUpdatable). CollisionManager stays a pure geometric solver: it reports contacts
// (colliders, normal, depth) and knows nothing about mass, velocity, or body types. All of that lives here.
//
// One Step(fixedDeltaTime):
//   1. Integrate  - every simulated RigidBody2D::Step(dt, gravity)
//   2. Detect     - CollisionManager::DetectContacts(); this first pass is kept for event dispatch
//   3. Solve      - up to m_solverIterations passes: resolve each non-trigger contact, then re-detect
//   4. Dispatch   - CollisionManager::DispatchContactEvents() using the FIRST (pre-solve) contacts, so a body
//                   resting on a surface reports a steady "Update" rather than flickering Enter/Exit
//
// Resolution rules (discrete):
//   - A collider with no RigidBody2D on its GameObject behaves as Static
//   - Only Dynamic bodies are moved by contact resolution (inverse mass = 1 / mass); Kinematic and Static have inverse mass 0
//   - Positional correction is split by inverse mass, so heavier bodies are pushed less and Static bodies never move
//   - Pairs with a combined inverse mass of 0 (e.g. Static vs Kinematic) are skipped
//   - Triggers generate events only

namespace CE
{
	class CollisionManager;
	class Collider2D;
	class GameObject;
	class RigidBody2D;

	class PhysicsManager : public Manager
	{
	private:
		static constexpr int kDefaultSolverIterations = 4;									// Default number of contact-solver passes per step

		std::vector<RigidBody2D*> m_bodies;													// All registered bodies (not owned)
		std::unordered_map<const GameObject*, RigidBody2D*> m_bodyByOwner;					// Owner -> body lookup, so contacts find their body without dynamic_cast

		std::vector<Contact2D> m_eventContacts;												// Contacts from the first detection pass of the current step (used for events)

		CollisionManager* m_pCollisionManager;												// Pointer to the CollisionManager (not owned)
		Vector2f m_gravity;																	// World gravity in world-units per second squared
		int m_solverIterations;																// Number of contact-solver passes per step

		RigidBody2D* FindBody(const Collider2D* pCollider) const;							// Returns the RigidBody2D on the collider's GameObject, or nullptr (treated as Static)
		static float GetInverseMass(const RigidBody2D* pBody);								// 1 / mass for Dynamic bodies, 0 for everything else (including nullptr)

		void IntegrateBodies(float fixedDeltaTime);											// Steps every simulated RigidBody2D
		bool SolveContacts(const std::vector<Contact2D>& contacts);							// Resolves all non-trigger contacts; returns true if any were resolved
		void ResolvePosition(const Contact2D& contact,
			RigidBody2D* pBodyA, RigidBody2D* pBodyB);										// Separates a pair, weighted by inverse mass
		void ResolveVelocity(const Contact2D& contact,
			RigidBody2D* pBodyA, RigidBody2D* pBodyB);										// Removes closing velocity along the normal (restitution comes from the combined materials)

	public:
		PhysicsManager();																	// Constructor
		~PhysicsManager() override;															// Destructor
		PhysicsManager(const PhysicsManager&) = delete;										// Prevent copy-construction
		PhysicsManager& operator=(const PhysicsManager&) = delete;							// Prevent copy-assignment
		PhysicsManager(PhysicsManager&&) = delete;											// Prevent move-construction
		PhysicsManager& operator=(PhysicsManager&&) = delete;								// Prevent move-assignment

		bool Initialize() override;															// Caches the CollisionManager
		void Step(float fixedDeltaTime);													// Advances the simulation by one fixed step (called from EngineManager's accumulator)
		void Shutdown() override;															// Drops all registered bodies

		void AddBody(RigidBody2D* pBody);													// Registers a body (called from RigidBody2D::Initialize)
		void RemoveBody(RigidBody2D* pBody);												// Unregisters a body (called from ~RigidBody2D)

		const Vector2f& GetGravity() const;													// Returns the world gravity
		void SetGravity(const Vector2f& gravity);											// Sets the world gravity

		int GetSolverIterations() const;													// Returns the number of contact-solver passes per step
		void SetSolverIterations(int iterations);											// Sets the number of contact-solver passes per step (clamped to >= 1)
	};
}