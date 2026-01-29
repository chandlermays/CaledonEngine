#include "Square.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"

CE::Square::Square(const Color& color, int size, bool isFilled)
	: Shape{ ShapeType::kSquare, color, size, size, isFilled }
{ }

void CE::Square::Render(Renderer* pRenderer, const Rect& destRect) const
{
	if (!pRenderer)
		return;

	if (m_isFilled)
	{
		pRenderer->DrawFilledRect(destRect, m_color);
	}
	else
	{
		pRenderer->DrawRect(destRect, m_color);
	}
}