/*------------------------------
| File: SDLTexture.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Rendering/Texture.h"

struct SDL_Texture;

namespace CE
{
	class SDLTexture : public Texture
	{
	private:
		SDL_Texture* m_pTexture;									// Pointer to the SDL Texture
		int m_width;												// Width of the texture
		int m_height;												// Height of the texture

	public:
		SDLTexture();												// Constructor
		~SDLTexture() override;										// Destructor

		bool Load(SDL_Texture* pTexture, int width, int height);	// Loads the SDL texture and sets dimensions

		int GetWidth() const override;								// Returns the width of the texture
		int GetHeight() const override;								// Returns the height of the texture
		void* GetNativeHandle() const override;						// Returns the native texture handle
	};
}