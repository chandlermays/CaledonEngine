#pragma once
#include "CaledonEngine/Engine/Manager.h"

namespace CE
{
	class SceneManager : public Manager
	{
	private:


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
	};
}