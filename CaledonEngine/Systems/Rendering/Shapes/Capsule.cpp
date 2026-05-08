/*------------------------------
| File: Capsule.cpp
| Author: Chandler Mays
------------------------------*/
#include "Capsule.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Utilities/Math/Rect.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*---------------------------------------------------------------------
| --- Constructor: Constructs a Capsule shape with default values --- |
---------------------------------------------------------------------*/
CE::Capsule::Capsule(const Color& color, int width, int height, bool isFilled)
	: Shape{ ShapeType::kCapsule, color, width, height, isFilled }
{ }

/*-------------------------------------------------------------------------------
| --- Render: Draws the capsule onto the screen using the provided Renderer --- |
-------------------------------------------------------------------------------*/
void CE::Capsule::Render(Renderer* pRenderer, const Rect& destRect, const Color& color) const
{
	if (!pRenderer)
		return;

	int centerX = destRect.m_x + destRect.m_width / 2;
	int centerY = destRect.m_y + destRect.m_height / 2;

	pRenderer->DrawCapsule(centerX, centerY, destRect.m_width, destRect.m_height, color, m_isFilled);
}