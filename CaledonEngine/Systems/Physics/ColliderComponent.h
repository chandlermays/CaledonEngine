#pragma once
#include "CaledonEngine/Core/Component.h"
#include "CaledonEngine/Utilities/Math/Vector2.h"
#include "CaledonEngine/Utilities/Math/Rect.h"
#include <functional>

namespace CE
{
	enum class ColliderType
	{
		kBox2D,																				// A 2D box-shaped collider
		kCircle2D,																			// A 2D circle-shaped collider
		kCapsule2D																			// A 2D capsule-shaped collider
	};

	class ColliderComponent : public Component
	{
	public:
		using CollisionCallback = std::function<void(ColliderComponent*)>;					// Type alias for collision callback functions

	protected:
		ColliderType m_colliderType;														// The type of the collider
		Rect m_bounds;																		// The world space bounding area of the collider
		Vector2f m_offset;																	// The local offset of the collider geometry
		bool m_isTrigger;																	// Whether the collider is a trigger or not

		std::vector<ColliderComponent*> m_overlappingColliders;								// List of colliders currently overlapping with this collider

		CollisionCallback m_onCollisionEnter;												// Callback for collision enter event
		CollisionCallback m_onCollisionUpdate;												// Callback for collision update event
		CollisionCallback m_onCollisionExit;												// Callback for collision exit event
		CollisionCallback m_onTriggerEnter;													// Callback for trigger enter event
		CollisionCallback m_onTriggerUpdate;												// Callback for trigger update event
		CollisionCallback m_onTriggerExit;													// Callback for trigger exit event

		ColliderComponent();																// Constructor

	public:
		virtual ~ColliderComponent();														// Destructor
		ColliderComponent(const ColliderComponent&) = delete;								// Prevent copy-construction
		ColliderComponent& operator=(const ColliderComponent&) = delete;					// Prevent copy-assignment
		ColliderComponent(ColliderComponent&&) = delete;									// Prevent move-construction
		ColliderComponent& operator=(ColliderComponent&&) = delete;							// Prevent move-assignment

		virtual bool Initialize() override;													// Initializes the ColliderComponent
		virtual void Update(float deltaTime) override;										// Updates the collider bounds based on owner's position

		std::vector<ColliderComponent*> Overlap() const;									// Returns a list of all colliders that overlap this collider
		bool IsTouching(const ColliderComponent* pOther) const;								// Returns whether this collider is touching the collider or not
		virtual bool ContainsPoint(const Vector2f& point) const = 0;						// Returns whether the given point is inside the collider or not
		virtual Vector2f ClosestPoint(const Vector2f& point) const = 0;						// Returns the closest point on the collider to the given point

		virtual void SetBounds(const Rect& bounds);											// Sets the world space bounding area of the collider
		virtual void SetOffset(const Vector2f& offset);										// Sets the local offset of the collider geometry
		virtual void SetTrigger(bool isTrigger);											// Sets whether the collider is a trigger or not
		virtual void UpdateBounds();														// Updates the world space bounding area of the collider based on the owner's position and local offset

		virtual ColliderType GetColliderType() const;										// Returns the type of the collider
		virtual Rect GetBounds() const;														// Returns the world space bounding area of the collider
		virtual Vector2f GetOffset() const;													// Returns the local offset of the collider geometry
		virtual bool IsTrigger() const;														// Returns whether the collider is a trigger or not

		void SetOnCollisionEnterCallback(CollisionCallback callback);						// Sets the callback for collision enter event
		void SetOnCollisionUpdateCallback(CollisionCallback callback);						// Sets the callback for collision update event
		void SetOnCollisionExitCallback(CollisionCallback callback);						// Sets the callback for collision exit event
		void SetOnTriggerEnterCallback(CollisionCallback callback);							// Sets the callback for trigger enter event
		void SetOnTriggerUpdateCallback(CollisionCallback callback);						// Sets the callback for trigger update event
		void SetOnTriggerExitCallback(CollisionCallback callback);							// Sets the callback for trigger exit event

		virtual void DebugDraw() const;														// Renders debug information for the collider
	};
}