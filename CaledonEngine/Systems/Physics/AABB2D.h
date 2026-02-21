#pragma once
#include "CaledonEngine/Utilities/Math/Vector2.h"

namespace CE
{
	struct AABB2D
	{
		// The minimum point of the AABB (x_min, y_min)
		// The maximum point of the AABB (x_max, y_max)
		Vector2f min;
		Vector2f max;

		// Constructor: AABB2D()														region R = { (x, y, z) | x_min <= x <= x_max, y_min <= y <= y_max, z = 0 }
		// Constructor: AABB2D(const Vector2& min, const Vector2& max)					region R = { (x, y, z) | min.x <= x  min.x + dx, min.y <= y <= min.y + dy, min.z <= z <= min.z + dz }
		// Constructor: AABB2D(const Vector2& center, const float radius)				region R = { (x, y, z) | c.x - x | <= rx, | c.y - y | <= ry, | c.z - z | <= rz }

		// operator==
		// operator!=

		// methods:
		// bool IsValid() const									-	returns true if the min point is less than the max point
		// void Reset()											-	resets the bounding box to the empty state (min = (inf, inf), max = (-inf, -inf))
		// void Encapsulate(const Vectro2& point)				-	grow the box (if necessary) to include the given point
		// void Encapsulate(const AABB2D& box)					-	grow the box (if necessary) to include the given box
		// AABB2D Intersect(const AABB2D& other) const			-	intersect this bounding box with the other, returning that intersection
		// void ExpandBy(const Vector2 distance)				-   widen the box on both sides by the given distance
		// Vector2 GetCenter() const							-	returns the center point of the box
		// Vector2 GetSize() const								-	returns the size of the box
		// Vector2 GetExtents() const							-	returns the extents of the box (half the size)
		// float GetArea() const								-	returns the area of the box
		// bool Contains(const AABB2D& other) const				-   returns true if the other box is inside this box
		// bool Contains(const Vector2 point) const				-   returns true if the point is inside this box
		// bool Overlaps(const AABB2D& other) const				-   returns true if the other box overlaps with this box
		// void Translate(const Vector2 translation)			-   moves the box by the given translation vector
		// AABB2D Scale(const Vector2 scale) const				-   scale this bounding box by the given scale factor
		// Vector2 GetClosestPoint(const Vector2 point) const	-   returns the closest point on the box to the given point
		// float DistanceTo(const Vector2 point) const			-   returns the distance from the box to the given point
		// float DistanceToSqr(const Vector2 point) const		-   returns the squared distance from the box to the given point
	};
}