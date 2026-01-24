#pragma once
#include "CaledonEngine/Systems/Rendering/Renderer.h"

struct SDL_Renderer;

namespace CE
{
	class SDLRenderer : public Renderer
	{
	private:
		SDL_Renderer* m_pRenderer;										// Pointer to the SDL Renderer

	public:
		SDLRenderer() = default;										// Constructor
		~SDLRenderer() override;										// Destructor
		SDLRenderer(const SDLRenderer&) = delete;						// Prevent copy-construction
		SDLRenderer& operator=(const SDLRenderer&) = delete;			// Prevent copy-assignment
		SDLRenderer(SDLRenderer&&) = delete;							// Prevent move-construction
		SDLRenderer& operator=(SDLRenderer&&) = delete;					// Prevent move-assignment

		bool Initialize(Window* pWindow) override;						// Prepares the SDL Renderer for use
		void Shutdown() override;										// Cleans up the SDL Renderer
		void Render() override;											// Executes the rendering process

		void* GetNativeHandle() const override;							// Retrieves the native SDL Renderer handle
	};
}