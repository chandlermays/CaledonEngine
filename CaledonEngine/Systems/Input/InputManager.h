/*------------------------------
| File: InputManager.h
| Author: Chandler Mays
------------------------------*/
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
		std::unique_ptr<Input> m_pInputAPI;										// Pointer to the Input's API configuration
		InputActions* m_pInputActions;											// Pointer to the Input Actions

		void ProcessInputActions();												// Processes all input actions
		void ProcessAction(InputAction* pAction);								// Processes a single input action
		float CalculateAxisValue(InputAction* pAction);							// Calculates the axis value for a Value type input action

	public:
		InputManager();															// Constructor
		~InputManager() = default;												// Destructor
		InputManager(const InputManager&) = delete;								// Prevent copy-construction
		InputManager& operator=(const InputManager&) = delete;					// Prevent copy-assignment
		InputManager(InputManager&&) = delete;									// Prevent move-construction
		InputManager& operator=(InputManager&&) = delete;						// Prevent move-assignment

		bool Initialize() override;												// Prepares the Input Manager for use
		void Shutdown() override;												// Cleans up and shuts down the input system
		bool ProcessEvents();													// Processes input events from the input API

		void SetInputActions(InputActions* pInputActions);						// Assigns the Input Actions to the Input Manager

		Input* GetInputAPI() const;												// Returns a pointer to the Input's API
		InputActions* GetInputActions() const;									// Returns the assigned Input Actions
	};
}