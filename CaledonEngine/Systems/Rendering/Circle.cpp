#include "Circle.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------
| --- Constructor: Constructs the Circle with default values --- |
----------------------------------------------------------------*/
CE::Circle::Circle(const Color& color, int radius, bool isFilled)
	: Shape(ShapeType::kCircle, color, radius * 2, radius * 2, isFilled)
	, m_radius{ radius }
{
	// Update bounds to reflect the circle's dimensions
	m_bounds = Rect{ 0, 0, radius * 2, radius * 2 };
}

/*--------------------------------------------------
| --- Render: Draws the circle onto the screen --- |
--------------------------------------------------*/
void CE::Circle::Render(Renderer* pRenderer, const Rect& destRect) const
{
	if (pRenderer == nullptr)
		return;

	// Calculate the center position of the circle
	int centerX = destRect.m_x + destRect.m_width / 2;
	int centerY = destRect.m_y + destRect.m_height / 2;

	// Calculate the radius based on the destination rectangle
	// Use the smaller of width/height to ensure the circle fits
	int scaledRadius = (destRect.m_width < destRect.m_height ? destRect.m_width : destRect.m_height) / 2;

	// Draw the circle using the renderer
	if (m_isFilled)
	{
		pRenderer->DrawFilledCircle(centerX, centerY, scaledRadius, m_color);
	}
	else
	{
		pRenderer->DrawCircle(centerX, centerY, scaledRadius, m_color);
	}
}

/*-------------------------------------------------
| --- SetRadius: Sets the radius of the circle --- |
-------------------------------------------------*/
void CE::Circle::SetRadius(int radius)
{
	m_radius = radius;
	m_width = radius * 2;
	m_height = radius * 2;
	m_bounds = Rect{ m_bounds.m_x, m_bounds.m_y, radius * 2, radius * 2 };
}

/*---------------------------------------------------
| --- GetRadius: Returns the radius of the circle --- |
---------------------------------------------------*/
int CE::Circle::GetRadius() const
{
	return m_radius;
}