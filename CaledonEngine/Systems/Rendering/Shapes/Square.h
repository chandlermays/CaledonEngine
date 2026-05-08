#pragma once
#include "CaledonEngine/Systems/Rendering/Shapes/Shape.h"

namespace CE
{
	class Square : public Shape
	{
	public:
		Square(const Color& color = Color::White(), int size = 100, bool isFilled = true);				// Constructor
		~Square() override = default;																	// Destructor

		void Render(Renderer* pRenderer, const Rect& destRect, const Color& color) const override;		// Draws the square onto the screen
	};
}