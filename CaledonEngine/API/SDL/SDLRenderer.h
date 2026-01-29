#pragma once
#include "CaledonEngine/Systems/Rendering/Renderer.h"

struct SDL_Renderer;

namespace CE
{
	class SDLRenderer : public Renderer
	{
	private:
		SDL_Renderer* m_pRenderer;																	// Pointer to the SDL Renderer

	public:
		SDLRenderer();																				// Constructor
		~SDLRenderer() override;																	// Destructor

		bool Initialize(Window* pWindow) override;													// Prepares the SDL Renderer for use
		void Shutdown() override;																	// Cleans up the SDL Renderer
		void Render() override;																		// Executes the rendering process

		virtual void RenderCopy(Texture* pTexture, Rect* pSrc, Rect* pDest) override;				// Renders a texture to the screen

		virtual void SetTextureColorMod(Texture* pTexture, unsigned char r,
			unsigned char g, unsigned char b) override;												// Sets the color modulation for a texture

		virtual void SetTextureAlphaMod(Texture* pTexture, unsigned char a) override;				// Sets the alpha modulation for a texture

		virtual void DrawRect(const Rect& rect, const Color& color) override;						// Draws an outlined rectangle
		virtual void DrawFilledRect(const Rect& rect, const Color& color) override;					// Draws a filled rectangle

		virtual void DrawCircle(int centerX, int centerY, int radiusX, int radiusY,
			const Color& color, int segments) override;												// Draws an outlined circle
		virtual void DrawFilledCircle(int centerX, int centerY, int radiusX, int radiusY,
			const Color& color, int segments) override;												// Draws a filled circle

		virtual void DrawTriangle(const VectorInt& v1, const VectorInt& v2,
			const VectorInt& v3, const Color& color) override;										// Draws an outlined triangle
		virtual void DrawFilledTriangle(const VectorInt& v1, const VectorInt& v2,
			const VectorInt& v3, const Color& color) override;										// Draws a filled triangle

		virtual void DrawCapsule(int centerX, int centerY, int width, int height,
			const Color& color, int segments) override;												// Draws an outlined capsule
		virtual void DrawFilledCapsule(int centerX, int centerY, int width, int height,
			const Color& color, int segments) override;												// Draws a filled capsule

		void* GetNativeHandle() const override;														// Returns the native renderer handle
	};
}