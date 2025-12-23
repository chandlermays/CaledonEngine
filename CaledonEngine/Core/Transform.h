#pragma once
#include "CaledonEngine/Utilities/Math/Vector2.h"

namespace CE
{
	class Transform
	{
	private:
		VectorFloat m_position;																		// The position of the object
		float m_rotation;																			// The rotation of the object
		VectorFloat m_scale;																		// The scale of the object

	public:
		Transform();																				// Constructor
		Transform(const VectorFloat& position, float rotation, const VectorFloat& scale);			// Parameterized Constructor
		~Transform() = default;																		// Destructor
		Transform(const Transform&) = delete;														// Prevent copy-construction
		Transform& operator=(const Transform&) = delete;											// Prevent copy-assignment
		Transform(Transform&&) = delete;															// Prevent move-construction
		Transform& operator=(Transform&&) = delete;													// Prevent move-assignment

		const VectorFloat& GetPosition() const;														// Get the position of the object
		void SetPosition(const VectorFloat& position);												// Set the position of the object

		float GetRotation() const;																	// Get the rotation of the object
		void SetRotation(float rotation);															// Set the rotation of the object

		const VectorFloat& GetScale() const;														// Get the scale of the object
		void SetScale(const VectorFloat& scale);													// Set the scale of the object
	};
}