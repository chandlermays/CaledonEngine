#pragma once
#include "InputAction.h"
#include <unordered_map>
#include <string>

// This class represents the map (or container) of Input Actions, typically associated with a specific context (eg. Gameplay, UI, etc.)

namespace CE
{
	class InputActionMap
	{
	private:
		std::unordered_map<std::string, InputAction*> m_actions;
		std::string m_name;
		bool m_isEnabled;

	public:
		InputActionMap(const std::string& name);
		~InputActionMap() = default;
		InputActionMap(const InputActionMap&) = delete;
		InputActionMap& operator=(const InputActionMap&) = delete;
		InputActionMap(InputActionMap&&) = delete;
		InputActionMap& operator=(InputActionMap&&) = delete;

		void AddAction(const std::string& name, ActionType type = ActionType::kButton);
		void RemoveAction(const std::string& name);

		void Enable();
		void Disable();
		bool Contains(const std::string& name) const;

		const std::unordered_map<std::string, InputAction*>& GetInputActions() const;
		const std::string& GetName() const;
		InputAction* GetActionByName(const std::string& name) const;
		bool IsEnabled() const;
	};
}