/*------------------------------
| File: Contact2D.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Utilities/Math/Vector2.h"

// Describes one overlapping collider pair found by CollisionManager.
// Purely geometric: no mass, velocity, or body-type information lives here.

namespace CE
{
	class Collider2D;

	struct Contact2D
	{
		Collider2D* pColliderA;															// First collider of the pair
		Collider2D* pColliderB;															// Second collider of the pair
		Vector2f normal;																// Unit vector pointing from B to A (same convention as ColliderDistance2D)
		float depth;																	// Penetration depth (>= 0); moving A by normal * depth separates the pair
		bool isTrigger;																	// True if either collider is a trigger (events only, never resolved)

		constexpr Contact2D()															// Constructor
			: pColliderA{ nullptr }
			, pColliderB{ nullptr }
			, normal{}
			, depth{ 0.0f }
			, isTrigger{ false }
		{
		}

		constexpr Contact2D(Collider2D* pA, Collider2D* pB,
			const Vector2f& contactNormal, float penetrationDepth, bool trigger)		// Parameterized Constructor
			: pColliderA{ pA }
			, pColliderB{ pB }
			, normal{ contactNormal }
			, depth{ penetrationDepth }
			, isTrigger{ trigger }
		{
		}

		constexpr Vector2f GetMTV() const { return normal * depth; }					// Minimum Translation Vector that moves A out of B
	};
}