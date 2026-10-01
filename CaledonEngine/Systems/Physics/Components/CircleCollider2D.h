/*------------------------------
| File: CircleCollider2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Physics/Components/Collider2D.h"

namespace CE
{
	class CircleCollider2D : public Collider2D
	{
	private:
		float m_radius;																					// The radius of the circle collider

	protected:
		void RecalculateBounds() override;																// Recalculates the AABB based on the GameObject's transform and the circle collider's properties
		ColliderDistance2D ResolveOverlapDistance(const Collider2D& other) const override;				// Returns the distance between this circle collider and another collider

	public:
		CircleCollider2D();																				// Constructor
		~CircleCollider2D() override = default;															// Destructor

		Vector2f ClosestPoint(const Vector2f& point) const override;									// Returns the closest point on the circle collider's surface to a given point in world space
		bool Overlaps(const Collider2D& other) const override;											// Returns true if this circle collider overlaps with another collider

		void SetRadius(float radius);																	// Sets the radius of the circle collider
		float GetRadius() const;																		// Returns the radius of the circle collider

		const std::string& GetTypeName() const override;												// Returns the type name of the Component
	};
}