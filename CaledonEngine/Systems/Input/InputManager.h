#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include "CaledonEngine/Systems/Input/Input.h"
#include "CaledonEngine/Systems/Input/InputActions.h"

#include <memory>

namespace CE
{
	class InputManager : public Manager
	{
	private:
		std::unique_ptr<Input> m_pInputAPI;										// Pointer to the Input's API
		InputActions* m_pInputActions;											// Pointer to the Input Actions

		void ProcessInputActions();
		void ProcessAction(InputAction* pAction);
		float CalculateAxisValue(InputAction* pAction);

	public:
		InputManager();															// Constructor
		~InputManager() = default;												// Destructor
		InputManager(const InputManager&) = delete;								// Prevent copy-construction
		InputManager& operator=(const InputManager&) = delete;					// Prevent copy-assignment
		InputManager(InputManager&&) = delete;									// Prevent move-construction
		InputManager& operator=(InputManager&&) = delete;						// Prevent move-assignment

		bool Initialize() override;												// Initialize the input manager
		void Shutdown() override;												// Shutdown the input manager

		bool ProcessEvents();													// Process input events

		void SetInputActions(InputActions* pInputActions);
		InputActions* GetInputActions() const;
	};
}