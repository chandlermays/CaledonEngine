#pragma once
#include "CaledonEngine/Systems/Rendering/Shape.h"

namespace CE
{
	class Circle : public Shape
	{
	private:
		int m_segments;																	// The number of segments to approximate the circle

	public:
		Circle(const Color& color = Color::White(),
			int radius = 50, bool isFilled = true, int segments = 32);					// Constructor
		~Circle() override = default;													// Destructor

		void Render(Renderer* pRenderer, const Rect& destRect) const override;			// Draws the circle onto the screen

		void SetSegments(int segments);													// Sets the number of segments
		int GetSegments() const;														// Returns the number of segments
	};
}