/*------------------------------
| File: EngineManager.h 
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Manager.h"
#include "IRenderable.h"
#include "IUpdatable.h"
#include <vector>
#include <memory>

namespace CE
{
	class GraphicsManager;
	class ResourceManager;
	class SceneManager;
	class InputManager;
	class ToolsManager;

	class EngineManager : public Manager
	{
	private:
		std::vector<std::unique_ptr<Manager>> m_pManagers;					// Vector of pointers to managers
		std::vector<IRenderable*> m_pRenderables;							// Vector of pointers to renderable managers
		std::vector<IUpdatable*> m_pUpdatables;								// Vector of pointers to updatable managers

		GraphicsManager* m_pGraphicsManager;								// Pointer to the graphics manager
		ResourceManager* m_pResourceManager;								// Pointer to the resource manager
		SceneManager* m_pSceneManager;										// Pointer to the scene manager
		InputManager* m_pInputManager;										// Pointer to the input manager
		ToolsManager* m_pToolsManager;										// Pointer to the tools manager

		bool m_isRunning;													// Flag to indicate if the engine is running

		EngineManager();													// Constructor

		void RegisterManager(std::unique_ptr<CE::Manager> pManager);		// Registers a manager with the engine

		void Update(float deltaTime);										// Updates all engine subsystem managers
		void Render();														// Renders all engine subsystem managers

	public:
		~EngineManager();													// Destructor
		EngineManager(const EngineManager&) = delete;						// Prevent copy-construction
		EngineManager& operator=(const EngineManager&) = delete;			// Prevent copy-assignment
		EngineManager(EngineManager&&) = delete;							// Prevent move-construction
		EngineManager& operator=(EngineManager&&) = delete;					// Prevent move-assignment

		static EngineManager& GetInstance();								// Returns the singleton instance of the EngineManager

		bool Initialize() override;											// Initializes all engine subsystem managers
		void Run();															// Main loop that updates and renders all engine subsystem managers
		void Shutdown() override;											// Shuts down and cleans up all engine subsystem managers
	
		GraphicsManager* GetGraphicsManager() const;						// Returns a pointer to the GraphicsManager
		ResourceManager* GetResourceManager() const;						// Returns a pointer to the ResourceManager
		SceneManager* GetSceneManager() const;								// Returns a pointer to the SceneManager
		InputManager* GetInputManager() const;								// Returns a pointer to the InputManager
		ToolsManager* GetToolsManager() const;								// Returns a pointer to the ToolsManager
	};
}