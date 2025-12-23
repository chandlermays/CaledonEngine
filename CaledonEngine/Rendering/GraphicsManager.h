#pragma once
#include "CaledonEngine/Engine/Manager.h"

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

		bool Initialize() override;												// Initialize the graphics manager
		void Update(float deltaTime) override;									// Update the graphics manager
		void Render() override;													// Render the current frame
		void Shutdown() override;												// Shutdown the graphics manager
	};
}