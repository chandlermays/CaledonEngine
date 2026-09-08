/*------------------------------
| File: BoxCollider2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Physics/Components/Collider2D.h"

namespace CE
{
	class BoxCollider2D : public Collider2D
	{
	private:
		Vector2f m_size;														// The size of the box collider (width, height)
		float m_edgeRadius;														// The radius of the rounded edges of the box collider

	protected:
		void RecalculateBounds() override;										// Recalculates the AABB based on the GameObject's transform and the box collider's properties

	public:
		BoxCollider2D();														// Constructor
		~BoxCollider2D() override = default;									// Destructor

		Vector2f ClosestPoint(const Vector2f& point) const override;			// Returns the closest point on the box collider's surface to a given point in world space
		bool Overlaps(const Collider2D& other) const override;					// Returns true if this box collider overlaps with another collider

		void SetSize(const Vector2f& size);										// Sets the size of the box collider and recalculates the bounds
		const Vector2f& GetSize() const;										// Returns the size of the box collider

		void SetEdgeRadius(float radius);										// Sets the radius of the rounded edges of the box collider
		float GetEdgeRadius() const;											// Returns the radius of the rounded edges of the box collider

		const std::string& GetTypeName() const override;						// Returns the type name of the Component
	};
}