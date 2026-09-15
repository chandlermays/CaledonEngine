/*------------------------------
| File: ViewportPanel.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <cstdint>
#include <memory>

namespace CE
{
	class Texture;
	class Image;
}

class EditorContext;
struct SDL_Texture;

class ViewportPanel
{
private:
	SDL_Texture* m_pRenderTarget;										// Off-screen render target for the viewport
	std::shared_ptr<CE::Texture> m_pViewportTexture;					// Wrapped texture for use with the Renderer API

	int m_viewportWidth;												// Current viewport width in pixels
	int m_viewportHeight;												// Current viewport height in pixels

	bool CreateRenderTarget(int width, int height);						// Creates an off-screen render target
	void DestroyRenderTarget();											// Destroys the render target

public:
	ViewportPanel();													// Constructor
	~ViewportPanel();													// Destructor
	ViewportPanel(const ViewportPanel&) = delete;						// Prevent copy-construction
	ViewportPanel& operator=(const ViewportPanel&) = delete;			// Prevent copy-assignment
	ViewportPanel(ViewportPanel&&) = delete;							// Prevent move-construction
	ViewportPanel& operator=(ViewportPanel&&) = delete;					// Prevent move-assignment

	bool Initialize();													// Initializes the ViewportPanel for use
	void Draw(EditorContext& context);									// Draws the Viewport panel with the rendered scene
	void Shutdown();													// Cleans up the render target
};
