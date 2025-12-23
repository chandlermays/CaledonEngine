#pragma once
#include "CaledonEngine/Engine/Manager.h"

namespace CE
{
	class InputManager : public Manager
	{
	private:


	public:
		InputManager();															// Constructor
		~InputManager();														// Destructor
		InputManager(const InputManager&) = delete;								// Prevent copy-construction
		InputManager& operator=(const InputManager&) = delete;					// Prevent copy-assignment
		InputManager(InputManager&&) = delete;									// Prevent move-construction
		InputManager& operator=(InputManager&&) = delete;						// Prevent move-assignment

		bool Initialize() override;												// Initialize the input manager
		void Update(float deltaTime) override;									// Update the input manager
		void Render() override;													// Render the current frame
		void Shutdown() override;												// Shutdown the input manager
	};
}