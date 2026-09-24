/*------------------------------
| File: SceneManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Engine/IRenderable.h"
#include "Systems/Engine/IUpdatable.h"

#include <vector>
#include <memory>

namespace CE
{
	class Scene;
	class Renderer;
	class Texture;

	class SceneManager : public Manager, public IRenderable, public IUpdatable
	{
	private:
		std::vector<std::unique_ptr<Scene>> m_pScenes;							// The list of scenes
		Scene* m_pCurrentScene;													// Pointer reference to the current scene

		Renderer* m_pRenderer;													// Pointer reference to the renderer
		Texture* m_pRenderTarget;												// Pointer reference to the render target texture

	public:
		SceneManager();															// Constructor
		~SceneManager();														// Destructor
		SceneManager(const SceneManager&) = delete;								// Prevent copy-construction
		SceneManager& operator=(const SceneManager&) = delete;					// Prevent copy-assignment
		SceneManager(SceneManager&&) = delete;									// Prevent move-construction
		SceneManager& operator=(SceneManager&&) = delete;						// Prevent move-assignment

		bool Initialize() override;												// Prepares the SceneManager for use
		void Update(float deltaTime) override;									// Update the current scene
		void Render() override;													// Render the current scene
		void Shutdown() override;												// Shutdown the SceneManager and clean up resources
	
		void UnloadAllScenes();

		void AddScene(std::unique_ptr<Scene> pScene);							// Add a scene to the list of scenes
		void SetCurrentScene(Scene* pScene);									// Sets the provided scene as the current scene
		Scene* GetCurrentScene() const;											// Returns the current scene
		int GetSceneIndex(Scene* pScene) const;									// Returns the index of the provided scene

		void SetRenderTarget(Texture* pTarget);									// Sets the render target texture
	};
}