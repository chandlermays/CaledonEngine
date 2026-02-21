#include "Triangle.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Utilities/Math/Rect.h"
#include "CaledonEngine/Utilities/Math/Vector2.h"

CE::Triangle::Triangle(const Color& color, int width, int height, bool isFilled)
	: Shape{ ShapeType::kTriangle, color, width, height, isFilled }
{ }

void CE::Triangle::Render(Renderer* pRenderer, const Rect& destRect) const
{
	if (!pRenderer)
		return;

	Vector2i v1(destRect.m_x + destRect.m_width / 2, destRect.m_y);
	Vector2i v2(destRect.m_x, destRect.m_y + destRect.m_height);
	Vector2i v3(destRect.m_x + destRect.m_width, destRect.m_y + destRect.m_height);

	pRenderer->DrawTriangle(v1, v2, v3, m_color, m_isFilled);
}