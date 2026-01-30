#include "Capsule.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Utilities/Math/Rect.h"

CE::Capsule::Capsule(const Color& color, int width, int height, bool isFilled)
	: Shape{ ShapeType::kCapsule, color, width, height, isFilled }
{
	// Update bounds to reflect the capsule's dimensions
	m_bounds = Rect{ 0, 0, width, height };
}

void CE::Capsule::Render(Renderer* pRenderer, const Rect& destRect) const
{
	if (!pRenderer)
		return;

	int centerX = destRect.m_x + destRect.m_width / 2;
	int centerY = destRect.m_y + destRect.m_height / 2;

	if (m_isFilled)
	{
		pRenderer->DrawFilledCapsule(centerX, centerY, destRect.m_width, destRect.m_height, m_color);
	}
	else
	{
		pRenderer->DrawCapsule(centerX, centerY, destRect.m_width, destRect.m_height, m_color);
	}
}