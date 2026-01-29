#pragma once
#include "CaledonEngine/Systems/Rendering/Shape.h"

namespace CE
{
	class Capsule : public Shape
	{
	private:
		int m_segments;																		// The number of segments to approximate the capsule

	public:
		Capsule(const Color& color = Color::White(),
			int width = 100, int height = 200, bool isFilled = true, int segments = 16);	// Constructor
		~Capsule() override = default;														// Destructor

		void Render(Renderer* pRenderer, const Rect& destRect) const override;				// Draws the capsule onto the screen

		void SetSegments(int segments);														// Sets the number of segments
		int GetSegments() const;															// Returns the number of segments
	};
}