#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include <vector>

namespace CE
{
	class Scene;

	class SceneManager : public Manager
	{
	private:
		std::vector<Scene*> m_pScenes;										// Vector of pointers to scenes
		Scene* m_pCurrentScene;												// Pointer to the current scene

	public:
		SceneManager();														// Constructor
		~SceneManager();													// Destructor
		SceneManager(const SceneManager&) = delete;							// Prevent copy-construction
		SceneManager& operator=(const SceneManager&) = delete;				// Prevent copy-assignment
		SceneManager(SceneManager&&) = delete;								// Prevent move-construction
		SceneManager& operator=(SceneManager&&) = delete;					// Prevent move-assignment

		bool Initialize() override;											// Initialize the scene manager
		void Update(float deltaTime) override;								// Update the scene manager
		void Render() override;												// Render the current frame
		void Shutdown() override;											// Shutdown the scene manager
	
		void AddScene(Scene* pScene);										// Add a scene to the manager
		void SetCurrentScene(Scene* pScene);								// Set the current scene
		Scene* GetCurrentScene() const;										// Get the current scene
		int GetSceneIndex(Scene* pScene) const;								// Get the index of a scene
	};
}