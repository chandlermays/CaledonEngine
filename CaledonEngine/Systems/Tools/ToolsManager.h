/*------------------------------
| File: ToolsManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Engine/IRenderable.h"
#include "Systems/Engine/IUpdatable.h"
#include "Systems/Tools/DebugOverlay.h"

namespace CE
{
	class InputManager;
	class Renderer;
	class Texture;

	class ToolsManager : public Manager, public IRenderable, public IUpdatable
	{
	private:
		InputManager* m_pInputManager;																// Pointer reference to the InputManager
		DebugOverlay m_debugOverlay;																// Instance of the DebugOverlay tool
		Renderer* m_pRenderer;																		// Pointer reference to the renderer
		Texture* m_pSceneRenderTarget;																// Pointer reference to the current scene's render target

	public:
		ToolsManager();																				// Constructor
		~ToolsManager();																			// Destructor
		ToolsManager(const ToolsManager&) = delete;													// Prevent copy-construction
		ToolsManager& operator=(const ToolsManager&) = delete;										// Prevent copy-assignment
		ToolsManager(ToolsManager&&) = delete;														// Prevent move-construction
		ToolsManager& operator=(ToolsManager&&) = delete;											// Prevent move-assignment

		bool Initialize() override;																	// Prepares the ToolsManager for use
		void Update(float) override;																// Checks for input to toggle the debug overlay
		void Render() override;																		// Renders the debug overlay using the current scene's render target
		void Shutdown() override;																	// Shutdown the ToolsManager and clean up resources

		DebugOverlay& GetDebugOverlay() { return m_debugOverlay; }									// Returns a reference to the DebugOverlay tool
	};
}