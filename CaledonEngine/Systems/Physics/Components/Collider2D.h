/*------------------------------
| File: Collider2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Core/Component.h"
#include "Systems/Physics/AABB2D.h"
#include "Utilities/Math/Vector2.h"

#include <functional>
#include <vector>

namespace CE
{
	class Collider2D : public Component
	{
	public:
		using CollisionCallback = std::function<void(Collider2D*)>;							// Type alias for a collision callback function that takes a pointer to another Collider2D as an argument

	protected:
		AABB2D m_bounds;																	// The Axis-Aligned Bounding Box (AABB) of the collider in world space
		Vector2f m_offset;																	// Offset of the collider's bounds relative to the GameObject's position
		bool m_isTrigger;																	// Whether the collider is a trigger (does not cause physical collisions, only events)

		std::vector<CollisionCallback> m_onEnterCallbacks;									// Callbacks to invoke when a collision starts
		std::vector<CollisionCallback> m_onUpdateCallbacks;									// Callbacks to invoke when a collision is ongoing
		std::vector<CollisionCallback> m_onExitCallbacks;									// Callbacks to invoke when a collision ends

		virtual void RecalculateBounds() = 0;												// Recalculates the AABB based on the GameObject's transform and any collider-specific properties

	public:
		Collider2D();																		// Constructor
		~Collider2D() override;																// Destructor
		Collider2D(const Collider2D&) = delete;												// Prevent copy-construction
		Collider2D& operator=(const Collider2D&) = delete;									// Prevent copy-assignment
		Collider2D(Collider2D&&) = delete;													// Prevent move-construction
		Collider2D& operator=(Collider2D&&) = delete;										// Prevent move-assignment

		bool Initialize() override;															// Prepares the Collider2D for use
		void Update(float deltaTime) override;												// Updates the Collider2D's bounds based on the GameObject's transform

		virtual Vector2f ClosestPoint(const Vector2f& point) const = 0;						// Returns the closest point on the collider's surface to a given point in world space
		float Distance(const Vector2f& point) const;										// Returns the distance from a given point in world space to the closest point on the collider's surface

		virtual bool Overlaps(const Collider2D& other) const = 0;							// Returns true if this collider overlaps with another collider

		const AABB2D& GetBounds() const;													// Returns the Axis-Aligned Bounding Box (AABB) of the collider in world space

		bool IsTrigger() const;																// Returns whether the collider is a trigger (does not cause physical collisions, only events)
		void SetTrigger(bool isTrigger);													// Sets whether the collider is a trigger (does not cause physical collisions, only events)

		const Vector2f& GetOffset() const;													// Returns the offset of the collider's bounds relative to the GameObject's position
		void SetOffset(const Vector2f& offset);												// Sets the offset of the collider's bounds relative to the GameObject's position and recalculates the bounds

		void OnCollisionEnter(CollisionCallback callback);									// Registers a callback to be invoked when a collision starts
		void OnCollisionUpdate(CollisionCallback callback);									// Registers a callback to be invoked when a collision is ongoing
		void OnCollisionExit(CollisionCallback callback);									// Registers a callback to be invoked when a collision ends

		void InvokeEnter(Collider2D* pOther);												// Invokes all registered collision enter callbacks with the other collider as an argument
		void InvokeUpdate(Collider2D* pOther);												// Invokes all registered collision update callbacks with the other collider as an argument
		void InvokeExit(Collider2D* pOther);												// Invokes all registered collision exit callbacks with the other collider as an argument
	};
}