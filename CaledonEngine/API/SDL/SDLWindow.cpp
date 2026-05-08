#include "SDLWindow.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "SDL.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------
| --- Constructor: Constructs the 'SDL' Window with default values --- |
----------------------------------------------------------------------*/
CE::SDLWindow::SDLWindow()
	: m_pWindow{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::SDLWindow::~SDLWindow()
{
	Shutdown();
}

/*-------------------------------------------------------
| --- Initialize: Prepares the 'SDL' Window for use --- |
------------------------------------------------------*/
bool CE::SDLWindow::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		CE_LOG("SDLWindow::Initialize - SDL could not initialize! SDL_Error: {}", SDL_GetError());
		return false;
	}

	Uint32 windowFlags = SDL_WINDOW_SHOWN;
	if (m_isFullscreen)
	{
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}

	m_pWindow = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, windowFlags);

	if (m_pWindow == nullptr)
	{
		CE_LOG("SDLWindow::Initialize - Window could not be created! SDL_Error: {}", SDL_GetError());
		return false;
	}

	return true;
}

/*-------------------------------------------------------------
| --- Shutdown: Cleans up and shuts down the 'SDL' Window --- |
-------------------------------------------------------------*/
void CE::SDLWindow::Shutdown()
{
	if (m_pWindow != nullptr)
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}

	SDL_Quit();
}

/*------------------------------------------------
| --- SetTitle: Sets the title of the window --- |
------------------------------------------------*/
void CE::SDLWindow::SetTitle(const std::string& title)
{
	m_title = title;

	if (m_pWindow)
	{
		SDL_SetWindowTitle(m_pWindow, m_title.c_str());
	}
}

/*----------------------------------------------
| --- SetSize: Sets the size of the window --- |
----------------------------------------------*/
void CE::SDLWindow::SetSize(int width, int height)
{
    m_width = width;
	m_height = height;

	if (m_pWindow)
	{
		SDL_SetWindowSize(m_pWindow, m_width, m_height);
	}
}

/*--------------------------------------------------
| --- SetFullscreen: Sets the fullscreen state --- |
--------------------------------------------------*/
void CE::SDLWindow::SetFullscreen(bool isFullscreen)
{
	m_isFullscreen = isFullscreen;
	if (m_isFullscreen)
	{
		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(m_pWindow, 0);
	}
}

/*--------------------------------
| --- Show: Shows the window --- |
--------------------------------*/
void CE::SDLWindow::Show()
{
	if (m_pWindow)
	{
		m_isMinimized = false;
		m_isFocused = true;
		SDL_ShowWindow(m_pWindow);
	}
}

/*--------------------------------
| --- Hide: Hides the window --- |
--------------------------------*/
void CE::SDLWindow::Hide()
{
	if (m_pWindow)
	{
		m_isMinimized = true;
		m_isFocused = false;
		SDL_HideWindow(m_pWindow);
	}
}

/*-----------------------------------------------------------
| --- GetNativeHandle: Returns the native window handle --- |
-----------------------------------------------------------*/
void* CE::SDLWindow::GetNativeHandle() const
{
	return static_cast<void*>(m_pWindow);
}