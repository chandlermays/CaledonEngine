#include "SDLRenderer.h"
#include "CaledonEngine/Systems/Rendering/Window.h"
#include "SDL.h"

CE::SDLRenderer::~SDLRenderer()
{
    Shutdown();
}

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

void CE::SDLRenderer::Shutdown()
{
    if (!m_pRenderer)
        return;

    SDL_DestroyRenderer(m_pRenderer);
    m_pRenderer = nullptr;
}

void CE::SDLRenderer::Render()
{
    if (!m_pRenderer)
        return;

	SDL_SetRenderDrawColor(m_pRenderer, 0, 156, 255, 255);
	SDL_RenderClear(m_pRenderer);
	SDL_RenderPresent(m_pRenderer);
}

void* CE::SDLRenderer::GetNativeHandle() const
{
	return static_cast<void*>(m_pRenderer);
}
