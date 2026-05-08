#pragma once
#include "Manager.h"
#include <vector>

namespace CE
{
	class GraphicsManager;
	class InputManager;
	class SceneManager;
	class ToolsManager;
	class ResourceManager;

	class EngineManager : public Manager
	{
	private:
		std::vector<Manager*> m_pManagers;									// Vector of pointers to managers
		GraphicsManager* m_pGraphicsManager;								// Pointer to the graphics manager
		InputManager* m_pInputManager;										// Pointer to the input manager
		SceneManager* m_pSceneManager;										// Pointer to the scene manager
		ToolsManager* m_pToolsManager;										// Pointer to the tools manager
		ResourceManager* m_pResourceManager;								// Pointer to the resource manager

		bool m_isRunning;													// Flag to indicate if the engine is running

		EngineManager();													// Constructor

		void Update(float deltaTime) override;								// Updates all engine subsystem managers
		void Render() override;												// Renders all engine subsystem managers

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
		InputManager* GetInputManager() const;								// Returns a pointer to the InputManager
		SceneManager* GetSceneManager() const;								// Returns a pointer to the SceneManager
		ToolsManager* GetToolsManager() const;								// Returns a pointer to the ToolsManager
		ResourceManager* GetResourceManager() const;						// Returns a pointer to the ResourceManager
	};
}