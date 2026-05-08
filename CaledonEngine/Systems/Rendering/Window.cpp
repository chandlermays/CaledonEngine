#include "Window.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------
| --- Constructor: Constructs the Window with default values --- |
----------------------------------------------------------------*/
CE::Window::Window(const std::string& title, int width, int height, bool isFullscreen)
	: m_title{ title }
	, m_width{ width }
	, m_height{ height }
	, m_isFullscreen{ isFullscreen }
	, m_isFocused{ true }
	, m_isMinimized{ false }
{ }

/*---------------------------------------------------
| --- GetTitle: Returns the title of the window --- |
---------------------------------------------------*/
const std::string& CE::Window::GetTitle() const
{
	return m_title;
}

/*---------------------------------------------------
| --- GetWidth: Returns the width of the window --- |
---------------------------------------------------*/
int CE::Window::GetWidth() const
{
	return m_width;
}

/*-----------------------------------------------------
| --- GetHeight: Returns the height of the window --- |
-----------------------------------------------------*/
int CE::Window::GetHeight() const
{
	return m_height;
}

/*------------------------------------------------------------------
| --- IsFullscreen: Checks if the window is in fullscreen mode --- |
------------------------------------------------------------------*/
bool CE::Window::IsFullscreen() const
{
	return m_isFullscreen;
}

/*----------------------------------------------------
| --- IsFocused: Checks if the window is focused --- |
----------------------------------------------------*/
bool CE::Window::IsFocused() const
{
	return m_isFocused;
}

/*--------------------------------------------------------
| --- IsMinimized: Checks if the window is minimized --- |
--------------------------------------------------------*/
bool CE::Window::IsMinimized() const
{
	return m_isMinimized;
}