#pragma once
#include "CaledonEngine/Systems/Rendering/Color.h"
#include "CaledonEngine/Utilities/Math/Rect.h"
#include "CaledonEngine/Utilities/Math/Vector2.h"

namespace CE
{
	class Window;
	class Texture;

	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;															// Destructor
		Renderer(const Renderer&) = delete;														// Prevent copy-construction
		Renderer& operator=(const Renderer&) = delete;											// Prevent copy-assignment
		Renderer(Renderer&&) = delete;															// Prevent move-construction
		Renderer& operator=(Renderer&&) = delete;												// Prevent move-assignment

		virtual bool Initialize(Window* pWindow) = 0;											// Prepares the Renderer for use
		virtual void Shutdown() = 0;															// Cleans up the Renderer
		virtual void Render() = 0;																// Executes the rendering process

		virtual void RenderCopy(Texture* pTexture, Rect* pSrc, Rect* pDest) = 0;				// Renders a texture to the screen

		virtual void SetTextureColorMod(Texture* pTexture, uint8_t r,
			uint8_t g, uint8_t b) = 0;															// Sets the color modulation for a texture

		virtual void SetTextureAlphaMod(Texture* pTexture, uint8_t a) = 0;						// Sets the alpha modulation for a texture

		virtual void DrawRect(const Rect& rect, const Color& color, bool filled) = 0;			// Draws an outlined rectangle

		virtual void DrawCircle(int centerX, int centerY, int radius,
			const Color& color, bool filled) = 0;												// Draws an outlined circle

		virtual void DrawTriangle(const Vector2i& v1, const Vector2i& v2,
			const Vector2i& v3, const Color& color, bool filled) = 0;							// Draws an outlined triangle

		virtual void DrawCapsule(int centerX, int centerY, int width, int height,
			const Color& color, bool filled) = 0;												// Draws an outlined capsule

		virtual void* GetNativeHandle() const = 0;												// Returns the native Renderer handle
	};
}