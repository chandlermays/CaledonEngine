#include "Capsule.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Utilities/Math/Rect.h"

CE::Capsule::Capsule(const Color& color, int width, int height, bool isFilled, int segments)
	: Shape{ ShapeType::kCapsule, color, width, height, isFilled }
	, m_segments{ segments }
{ }

void CE::Capsule::Render(Renderer* pRenderer, const Rect& destRect) const
{
	if (!pRenderer)
		return;

	int centerX = destRect.m_x + destRect.m_width / 2;
	int centerY = destRect.m_y + destRect.m_height / 2;

	if (m_isFilled)
	{
		pRenderer->DrawCapsule(centerX, centerY, destRect.m_width, destRect.m_height, m_color, m_segments);
	}
	else
	{
		pRenderer->DrawFilledCapsule(centerX, centerY, destRect.m_width, destRect.m_height, m_color, m_segments);
	}
}

void CE::Capsule::SetSegments(int segments)
{
	if (segments >= 3)
	{
		m_segments = segments;
	}
}

int CE::Capsule::GetSegments() const
{
	return m_segments;
}