#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include <vector>

namespace CE
{
	class Scene;

	class SceneManager : public Manager
	{
	private:
		std::vector<Scene*> m_pScenes;										// The list of scenes
		Scene* m_pCurrentScene;												// Pointer to the current scene

	public:
		SceneManager();														// Constructor
		~SceneManager();													// Destructor
		SceneManager(const SceneManager&) = delete;							// Prevent copy-construction
		SceneManager& operator=(const SceneManager&) = delete;				// Prevent copy-assignment
		SceneManager(SceneManager&&) = delete;								// Prevent move-construction
		SceneManager& operator=(SceneManager&&) = delete;					// Prevent move-assignment

		bool Initialize() override;											// Prepares the SceneManager for use
		void Update(float deltaTime) override;								// Update the current scene
		void Render() override;												// Render the current scene
		void Shutdown() override;											// Shutdown the SceneManager and clean up resources
	
		void AddScene(Scene* pScene);										// Add a scene to the list of scenes
		void SetCurrentScene(Scene* pScene);								// Sets the provided scene as the current scene
		Scene* GetCurrentScene() const;										// Returns the current scene
		int GetSceneIndex(Scene* pScene) const;								// Returns the index of the provided scene
	};
}