#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"

struct SDL_Window;
struct SDL_Renderer;

namespace CE
{
	class GraphicsManager : public Manager
	{
	private:
		SDL_Window* m_pWindow;													// Pointer to the SDL Window
		SDL_Renderer* m_pRenderer;												// Pointer to the SDL Renderer

	public:
		GraphicsManager();														// Constructor
		~GraphicsManager();														// Destructor
		GraphicsManager(const GraphicsManager&) = delete;						// Prevent copy-construction
		GraphicsManager& operator=(const GraphicsManager&) = delete;			// Prevent copy-assignment
		GraphicsManager(GraphicsManager&&) = delete;							// Prevent move-construction
		GraphicsManager& operator=(GraphicsManager&&) = delete;					// Prevent move-assignment

		bool Initialize() override;												// Sets up the system's graphcs and window
		void Render() override;													// Presents the current frame to the screen
		void Shutdown() override;												// Cleans up and shuts down the graphics system
		
		SDL_Renderer* GetRenderer() const { return m_pRenderer; }				// Get the SDL Renderer
	};
}