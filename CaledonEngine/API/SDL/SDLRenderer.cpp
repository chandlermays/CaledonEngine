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

/*-------------------------------------
| --- DrawRect: Draws a rectangle --- |
-------------------------------------*/
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

/*-------------------------------------------------------------
| --- GetNativeHandle: Returns the native renderer handle --- |
-------------------------------------------------------------*/
void* CE::SDLRenderer::GetNativeHandle() const
{
	return static_cast<void*>(m_pRenderer);
}
