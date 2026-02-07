#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include <vector>

namespace CE
{
	class ColliderComponent;
	struct Rect;

	class CollisionManager : public Manager
	{
	private:
		std::vector<ColliderComponent*> m_colliders;															// List of all registered colliders in the scene
		std::vector<std::pair<ColliderComponent*, ColliderComponent*>> m_previousCollisions;					// List of collider pairs that were colliding in the previous frame

		bool CheckCollision(ColliderComponent* pColliderA, ColliderComponent* pColliderB);						// Checks for collision between two colliders
		bool AABBIntersect(const Rect& a, const Rect& b);														// Checks for intersection between two axis-aligned bounding boxes
		void HandleCollision(ColliderComponent* pColliderA, ColliderComponent* pColliderB, bool isColliding);	// Handles the collision events between two colliders based on their current and previous collision states

	public:
		CollisionManager();																						// Constructor
		~CollisionManager();																					// Destructor
		CollisionManager(const CollisionManager&) = delete;														// Prevent copy-construction
		CollisionManager& operator=(const CollisionManager&) = delete;											// Prevent copy-assignment
		CollisionManager(CollisionManager&&) = delete;															// Prevent move-construction
		CollisionManager& operator=(CollisionManager&&) = delete;												// Prevent move-assignment

		bool Initialize() override;																				// Initialize the collision manager
		void Update(float deltaTime) override;																	// Updates the collision detection for all registered colliders
		void Shutdown() override;																				// Shutdown the collision manager

		void RegisterCollider(ColliderComponent* pCollider);													// Registers a collider with the collision manager
		void UnregisterCollider(ColliderComponent* pCollider);													// Unregisters a collider from the collision manager
	};
}