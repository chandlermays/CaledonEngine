#pragma once
#include "ColliderComponent.h"

namespace CE
{
	class BoxCollider2D : public ColliderComponent
	{
	private:
		int m_width;																// The width of the box collider
		int m_height;																// The height of the box collider

	public:
		BoxCollider2D();															// Constructor
		virtual ~BoxCollider2D() = default;											// Destructor
		
		bool Initialize() override;													// Initialize the BoxCollider2D
		
		bool ContainsPoint(const Vector2f& point) const override;					// Returns whether the given point is inside the box collider or not
		Vector2f ClosestPoint(const Vector2f& point) const;							// Returns the closest point on the box collider to the given point

		void SetSize(int width, int height);										// Sets the size of the box collider

		int GetWidth() const;														// Returns the width of the box collider
		int GetHeight() const;														// Returns the height of the box collider
	};
}