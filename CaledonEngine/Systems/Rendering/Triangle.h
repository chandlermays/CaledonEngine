#pragma once
#include "CaledonEngine/Systems/Rendering/Shape.h"

namespace CE
{
	class Triangle : public Shape
	{
	public:
		Triangle(const Color& color = Color::White(),
			int width = 100, int height = 100, bool isFilled = true);					// Constructor
		~Triangle() override = default;													// Destructor

		void Render(Renderer* pRenderer, const Rect& destRect) const override;			// Draws the triangle onto the screen
	};
}