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
		Vector2f m_size;															// The size of the circle collider (width, height)
		float m_radius;																// The radius of the circle collider

	protected:
		void RecalculateBounds() override;											// Recalculates the AABB based on the GameObject's transform and the circle collider's properties

	public:
		CircleCollider2D();															// Constructor
		~CircleCollider2D() override = default;										// Destructor

		Vector2f ClosestPoint(const Vector2f& point) const override;				// Returns the closest point on the circle collider's surface to a given point in world space
		bool Overlaps(const Collider2D& other) const override;						// Returns true if this circle collider overlaps with another collider

		void SetSize(const Vector2f& size);											// Sets the size of the circle collider and recalculates the bounds
		const Vector2f& GetSize() const;											// Returns the size of the circle collider

		void SetRadius(float radius);												// Sets the radius of the circle collider
		float GetRadius() const;													// Returns the radius of the circle collider

		const std::string& GetTypeName() const override;							// Returns the type name of the Component

	}
}