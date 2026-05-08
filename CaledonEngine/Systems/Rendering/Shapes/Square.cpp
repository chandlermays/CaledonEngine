#include "Square.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------
| --- Constructor: Constructs a Square shape with default values --- |
--------------------------------------------------------------------*/
CE::Square::Square(const Color& color, int size, bool isFilled)
	: Shape{ ShapeType::kSquare, color, size, size, isFilled }
{ }

/*------------------------------------------------------------------------------
| --- Render: Draws the square onto the screen using the provided Renderer --- |
------------------------------------------------------------------------------*/
void CE::Square::Render(Renderer* pRenderer, const Rect& destRect, const Color& color) const
{
    if (!pRenderer)
        return;

    pRenderer->DrawRect(destRect, color, m_isFilled);
}