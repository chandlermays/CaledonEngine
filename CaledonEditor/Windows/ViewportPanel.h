/*------------------------------
| File: ViewportPanel.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <memory>

namespace CE
{
	class Texture;
}

class EditorContext;

class ViewportPanel
{
private:
	std::shared_ptr<CE::Texture> m_pRenderTexture;									// The render texture used to display the scene in the viewport
	int m_width;																	// The width of the render texture
	int m_height;																	// The height of the render texture

	void EnsureRenderTexture(int width, int height);								// Ensures that the render texture is created and matches the specified dimensions

public:
	ViewportPanel();																// Constructor
	~ViewportPanel() = default;														// Destructor
	ViewportPanel(const ViewportPanel&) = delete;									// Prevent copy-construction
	ViewportPanel& operator=(const ViewportPanel&) = delete;						// Prevent copy-assignment
	ViewportPanel(ViewportPanel&&) = delete;										// Prevent move-construction
	ViewportPanel& operator=(ViewportPanel&&) = delete;								// Prevent move-assignment

	void Draw(EditorContext& context);												// Draws the viewport panel and handles rendering the scene to the render texture
	void Shutdown();																// Cleans up the viewport's render target
};