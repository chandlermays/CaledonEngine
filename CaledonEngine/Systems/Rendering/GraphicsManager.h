/*------------------------------
| File: GraphicsManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include "CaledonEngine/Systems/Rendering/Window.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include <memory>

namespace CE
{
	class GraphicsManager : public Manager
	{
	private:
		std::unique_ptr<Window> m_pWindowAPI;									// Pointer to the Window's API
		std::unique_ptr<Renderer> m_pRendererAPI;								// Pointer to the Renderer's API

	public:
		GraphicsManager();														// Constructor
		~GraphicsManager();														// Destructor
		GraphicsManager(const GraphicsManager&) = delete;						// Prevent copy-construction
		GraphicsManager& operator=(const GraphicsManager&) = delete;			// Prevent copy-assignment
		GraphicsManager(GraphicsManager&&) = delete;							// Prevent move-construction
		GraphicsManager& operator=(GraphicsManager&&) = delete;					// Prevent move-assignment

		bool Initialize() override;												// Prepares the GraphicsManager for use
		void Render() override;													// Presents the current frame to the screen
		void Shutdown() override;												// Cleans up and shuts down the graphics system

		Window* GetWindow() const;												// Returns a pointer to the Window's API
		Renderer* GetRenderer() const;											// Returns a pointer to the Renderer's API
	};
}