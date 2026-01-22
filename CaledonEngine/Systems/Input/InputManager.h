#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include "CaledonEngine/Systems/Input/Input.h"

#include <memory>

namespace CE
{
	class InputManager : public Manager
	{
	private:
		std::unique_ptr<Input> m_inputAPI;										// Pointer to the Input API

	public:
		InputManager();															// Constructor
		~InputManager() = default;												// Destructor
		InputManager(const InputManager&) = delete;								// Prevent copy-construction
		InputManager& operator=(const InputManager&) = delete;					// Prevent copy-assignment
		InputManager(InputManager&&) = delete;									// Prevent move-construction
		InputManager& operator=(InputManager&&) = delete;						// Prevent move-assignment

		bool Initialize() override;												// Initialize the input manager
		void Shutdown() override;												// Shutdown the input manager
	};
}