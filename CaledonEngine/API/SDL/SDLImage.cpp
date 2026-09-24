/*------------------------------
| File: SDLImage.cpp
| Author: Chandler Mays
------------------------------*/
#include "SDLImage.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------
| --- Constructor: Constructs the SDL_Image with default values --- |
-------------------------------------------------------------------*/
CE::SDLImage::SDLImage()
	: m_pSurface{ nullptr }
{ }

/*-----------------------------------
| --- Parameterized Constructor --- |
-----------------------------------*/
CE::SDLImage::SDLImage(SDL_Surface* surface)
	: m_pSurface{ surface }
{ }

/*--------------------
| --- Destructor --- |
--------------------*/
CE::SDLImage::~SDLImage()
{
	if (m_pSurface)
	{
		SDL_DestroySurface(m_pSurface);
	}
}

/*------------------------------------------
| --- GetW: Get the width of the image --- |
------------------------------------------*/
int CE::SDLImage::GetW() const
{
	return m_pSurface ? m_pSurface->w : 0;
}

/*-------------------------------------------
| --- GetH: Get the height of the image --- |
-------------------------------------------*/
int CE::SDLImage::GetH() const
{
	return m_pSurface ? m_pSurface->h : 0;
}

/*--------------------------------------------------
| --- CreateImage: Create an Image from a File --- |
--------------------------------------------------*/
std::unique_ptr<CE::SDLImage> CE::SDLImage::CreateImage(const std::string& filePath)
{
	SDL_Surface* pSurface = IMG_Load(filePath.c_str());
	if (!pSurface)
		return nullptr;

	return std::make_unique<SDLImage>(pSurface);
}

/*---------------------------------------------------------------------
| --- GetNativeHandle: Get the native image handle (SDL_Surface*) --- |
---------------------------------------------------------------------*/
void* CE::SDLImage::GetNativeHandle() const
{
	return static_cast<void*>(m_pSurface);
}