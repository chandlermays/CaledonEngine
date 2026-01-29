#include "Circle.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Utilities/Math/Rect.h"

CE::Circle::Circle(const Color& color, int radius, bool isFilled, int segments)
	: Shape{ ShapeType::kCircle, color, radius * 2, radius * 2, isFilled }
	, m_segments{ segments }
{ }

void CE::Circle::Render(Renderer* pRenderer, const Rect& destRect) const
{
	if (!pRenderer)
		return;

	int centerX = destRect.m_x + destRect.m_width / 2;
	int centerY = destRect.m_y + destRect.m_height / 2;
	int radiusX = destRect.m_width / 2;
	int radiusY = destRect.m_height / 2;

	if (m_isFilled)
	{
		pRenderer->DrawCircle(centerX, centerY, radiusX, radiusY, m_color, m_segments);
	}
	else
	{
		pRenderer->DrawFilledCircle(centerX, centerY, radiusX, radiusY, m_color, m_segments);
	}
}

void CE::Circle::SetSegments(int segments)
{
	if (segments >= 3)
	{
		m_segments = segments;
	}
}

int CE::Circle::GetSegments() const
{
	return m_segments;
}