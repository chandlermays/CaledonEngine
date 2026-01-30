#include "SDLRenderer.h"
#include "CaledonEngine/Systems/Rendering/Window.h"
#include "CaledonEngine/Systems/Rendering/Texture.h"
#include "SDL.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------
| --- Constructor: Constructs the 'SDL' Renderer with default values --- |
------------------------------------------------------------------------*/
CE::SDLRenderer::SDLRenderer()
	: m_pRenderer{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::SDLRenderer::~SDLRenderer()
{
    Shutdown();
}

/*---------------------------------------------------------
| --- Initialize: Prepares the 'SDL' Renderer for use --- |
---------------------------------------------------------*/
bool CE::SDLRenderer::Initialize(Window* pWindow)
{
    if (!pWindow)
        return false;

	SDL_Window* pSDLWindow = static_cast<SDL_Window*>(pWindow->GetNativeHandle());
    if (!pSDLWindow)
        return false;

	m_pRenderer = SDL_CreateRenderer(pSDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!m_pRenderer)
        return false;

	return true;
}

/*------------------------------------------------
| --- Shutdown: Cleans up the 'SDL' Renderer --- |
------------------------------------------------*/
void CE::SDLRenderer::Shutdown()
{
    if (!m_pRenderer)
        return;

    SDL_DestroyRenderer(m_pRenderer);
    m_pRenderer = nullptr;
}

/*------------------------------------------------
| --- Render: Executes the rendering process --- |
------------------------------------------------*/
void CE::SDLRenderer::Render()
{
    if (!m_pRenderer)
        return;

	SDL_SetRenderDrawColor(m_pRenderer, 0, 156, 255, 255);
	SDL_RenderPresent(m_pRenderer);
	SDL_RenderClear(m_pRenderer);
}

/*-----------------------------------------------------
| --- RenderCopy: Renders a texture to the screen --- |
-----------------------------------------------------*/
void CE::SDLRenderer::RenderCopy(Texture* pTexture, Rect* pSrc, Rect* pDest)
{
    if (!m_pRenderer || !pTexture || !pDest)
        return;

	SDL_Texture* pSDLTexture = static_cast<SDL_Texture*>(pTexture->GetNativeHandle());
	SDL_Rect srcRect;
	SDL_Rect destRect;
    if (pSrc)
    {
        srcRect.x = pSrc->m_x;
        srcRect.y = pSrc->m_y;
        srcRect.w = pSrc->m_width;
        srcRect.h = pSrc->m_height;
    }

	destRect.x = pDest->m_x;
	destRect.y = pDest->m_y;
	destRect.w = pDest->m_width;
	destRect.h = pDest->m_height;

	SDL_RenderCopy(m_pRenderer, pSDLTexture, pSrc ? &srcRect : nullptr, &destRect);
}

/*---------------------------------------------------------------------
| --- SetTextureColorMod: Sets the color modulation for a texture --- |
---------------------------------------------------------------------*/
void CE::SDLRenderer::SetTextureColorMod(Texture* pTexture, unsigned char r, unsigned char g, unsigned char b)
{
    if (!m_pRenderer || !pTexture)
        return;

    SDL_Texture* pSDLTexture = static_cast<SDL_Texture*>(pTexture->GetNativeHandle());
    SDL_SetTextureColorMod(pSDLTexture, r, g, b);
}

/*---------------------------------------------------------------------
| --- SetTextureAlphaMod: Sets the alpha modulation for a texture --- |
---------------------------------------------------------------------*/
void CE::SDLRenderer::SetTextureAlphaMod(Texture* pTexture, unsigned char a)
{
    if (!m_pRenderer || !pTexture)
        return;

    SDL_Texture* pSDLTexture = static_cast<SDL_Texture*>(pTexture->GetNativeHandle());
    SDL_SetTextureAlphaMod(pSDLTexture, a);
}

/*-----------------------------------------------
| --- DrawRect: Draws an outlined rectangle --- |
-----------------------------------------------*/
void CE::SDLRenderer::DrawRect(const Rect& rect, const Color& color)
{
    if (!m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

    SDL_Rect sdlRect
    {
        rect.m_x,
        rect.m_y,
        rect.m_width,
        rect.m_height
    };

	SDL_RenderDrawRect(m_pRenderer, &sdlRect);
}

/*--------------------------------------------------
| --- DrawFilledRect: Draws a filled rectangle --- |
--------------------------------------------------*/
void CE::SDLRenderer::DrawFilledRect(const Rect& rect, const Color& color)
{
    if (!m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

    SDL_Rect sdlRect
    {
        rect.m_x,
        rect.m_y,
        rect.m_width,
        rect.m_height
    };

	SDL_RenderFillRect(m_pRenderer, &sdlRect);
}

/*----------------------------------------------
| --- DrawCircle: Draws an outlined circle --- |
-----------------------------------------------*/
void CE::SDLRenderer::DrawCircle(int centerX, int centerY, int radius, const Color& color)
{
    if (!m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        // Draw 8 symmetric points
        SDL_RenderDrawPoint(m_pRenderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(m_pRenderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(m_pRenderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(m_pRenderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(m_pRenderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(m_pRenderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(m_pRenderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(m_pRenderer, centerX + x, centerY - y);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

/*-------------------------------------------------
| --- DrawFilledCircle: Draws a filled circle --- |
-------------------------------------------------*/
void CE::SDLRenderer::DrawFilledCircle(int centerX, int centerY, int radius, const Color& color)
{
    if (!m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        // Draw horizontal lines to fill the circle
        SDL_RenderDrawLine(m_pRenderer, centerX - x, centerY + y, centerX + x, centerY + y);
        SDL_RenderDrawLine(m_pRenderer, centerX - y, centerY + x, centerX + y, centerY + x);
        SDL_RenderDrawLine(m_pRenderer, centerX - x, centerY - y, centerX + x, centerY - y);
        SDL_RenderDrawLine(m_pRenderer, centerX - y, centerY - x, centerX + y, centerY - x);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

/*--------------------------------------------------
| --- DrawTriangle: Draws an outlined triangle --- |
--------------------------------------------------*/
void CE::SDLRenderer::DrawTriangle(const VectorInt& v1, const VectorInt& v2, const VectorInt& v3, const Color& color)
{
    if (!m_pRenderer)
        return;

	SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

	SDL_RenderDrawLine(m_pRenderer, v1.m_x, v1.m_y, v2.m_x, v2.m_y);
	SDL_RenderDrawLine(m_pRenderer, v2.m_x, v2.m_y, v3.m_x, v3.m_y);
	SDL_RenderDrawLine(m_pRenderer, v3.m_x, v3.m_y, v1.m_x, v1.m_y);
}

/*-----------------------------------------------------
| --- DrawFilledTriangle: Draws a filled triangle --- |
-----------------------------------------------------*/
void CE::SDLRenderer::DrawFilledTriangle(const VectorInt& v1, const VectorInt& v2, const VectorInt& v3, const Color& color)
{
    if (!m_pRenderer)
        return;

	SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

	int minY = std::min(std::min(v1.m_y, v2.m_y), v3.m_y);
	int maxY = std::max(std::max(v1.m_y, v2.m_y), v3.m_y);

    for (int y = minY; y <= maxY; ++y)
    {
        int minX = INT_MAX;
        int maxX = INT_MIN;

        auto checkEdge = [&](const VectorInt& p1, const VectorInt& p2)
            {
                if ((p1.m_y <= y && p2.m_y >= y) || (p2.m_y <= y && p1.m_y >= y))
                {
                    if (p2.m_y != p1.m_y)
                    {
                        int x = p1.m_x + (y - p1.m_y) * (p2.m_x - p1.m_x) / (p2.m_y - p1.m_y);
                        minX = std::min(minX, x);
                        maxX = std::max(maxX, x);
                    }
                }
            };

        checkEdge(v1, v2);
        checkEdge(v2, v3);
        checkEdge(v3, v1);

        if (minX != INT_MAX && maxX != INT_MIN)
        {
            SDL_RenderDrawLine(m_pRenderer, minX, y, maxX, y);
        }
    }
}

/*------------------------------------------------
| --- DrawCapsule: Draws an outlined capsule --- |
------------------------------------------------*/
void CE::SDLRenderer::DrawCapsule(int centerX, int centerY, int width, int height, const Color& color)
{
    if (!m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

    bool isVertical = height > width;

    if (isVertical)
    {
        // Vertical capsule
        int radius = width / 2;
        int rectHeight = height - width;

        // Top semicircle
        int topY = centerY - rectHeight / 2;
        DrawCircle(centerX, topY, radius, color);

        // Side lines
        int rectTop = topY;
        int rectBottom = topY + rectHeight;
        SDL_RenderDrawLine(m_pRenderer, centerX - radius, rectTop, centerX - radius, rectBottom);
        SDL_RenderDrawLine(m_pRenderer, centerX + radius, rectTop, centerX + radius, rectBottom);

        // Bottom semicircle
        int bottomY = centerY + rectHeight / 2;
        DrawCircle(centerX, bottomY, radius, color);
    }
    else
    {
        // Horizontal capsule
        int radius = height / 2;
        int rectWidth = width - height;

        // Left semicircle
        int leftX = centerX - rectWidth / 2;
        DrawCircle(leftX, centerY, radius, color);

        // Top and bottom lines
        int rectLeft = leftX;
        int rectRight = leftX + rectWidth;
        SDL_RenderDrawLine(m_pRenderer, rectLeft, centerY - radius, rectRight, centerY - radius);
        SDL_RenderDrawLine(m_pRenderer, rectLeft, centerY + radius, rectRight, centerY + radius);

        // Right semicircle
        int rightX = centerX + rectWidth / 2;
        DrawCircle(rightX, centerY, radius, color);
    }
}

/*---------------------------------------------------
| --- DrawFilledCapsule: Draws a filled capsule --- |
---------------------------------------------------*/
void CE::SDLRenderer::DrawFilledCapsule(int centerX, int centerY, int width, int height, const Color& color)
{
    if (!m_pRenderer)
        return;

    SDL_SetRenderDrawColor(m_pRenderer, color.m_r, color.m_g, color.m_b, color.m_a);

    bool isVertical = height > width;

    if (isVertical)
    {
        // Vertical capsule
        int radius = width / 2;
        int rectHeight = height - width;

        // Top semicircle
        int topY = centerY - rectHeight / 2;
        DrawFilledCircle(centerX, topY, radius, color);

        // Middle rectangle
        Rect middleRect(centerX - radius, topY, width, rectHeight);
        DrawFilledRect(middleRect, color);

        // Bottom semicircle
        int bottomY = centerY + rectHeight / 2;
        DrawFilledCircle(centerX, bottomY, radius, color);
    }
    else
    {
        // Horizontal capsule
        int radius = height / 2;
        int rectWidth = width - height;

        // Left semicircle
        int leftX = centerX - rectWidth / 2;
        DrawFilledCircle(leftX, centerY, radius, color);

        // Middle rectangle
        Rect middleRect(leftX, centerY - radius, rectWidth, height);
        DrawFilledRect(middleRect, color);

        // Right semicircle
        int rightX = centerX + rectWidth / 2;
        DrawFilledCircle(rightX, centerY, radius, color);
    }
}

/*-------------------------------------------------------------
| --- GetNativeHandle: Returns the native renderer handle --- |
-------------------------------------------------------------*/
void* CE::SDLRenderer::GetNativeHandle() const
{
	return static_cast<void*>(m_pRenderer);
}