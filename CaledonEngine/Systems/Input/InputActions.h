#pragma once
#include "InputActionMap.h"
#include <unordered_map>
#include <string>

// This is the class (or asset) that gets created in the Game project to define the input actions and their mappings to keys/buttons.

// Contains the action maps (eg. Gameplay, UI, etc.) and the input actions within those maps.

namespace CE
{
	class InputActions
	{
	private:
		std::unordered_map<std::string, InputActionMap*> m_actionMaps;
		std::string m_assetName;
		bool m_isEnabled;

	public:
		InputActions(const std::string& assetName = "NewInputActions");
		~InputActions() = default;
		InputActions(const InputActions&) = delete;
		InputActions& operator=(const InputActions&) = delete;
		InputActions(InputActions&&) = delete;
		InputActions& operator=(InputActions&&) = delete;

		void AddActionMap(const std::string& name);
		void RemoveActionMap(const std::string& name);

		const InputActionMap* GetActionMapByName(const std::string& name) const;
		const std::unordered_map<std::string, InputActionMap*>& GetInputActionMaps() const;
		bool IsEnabled() const;
		InputAction* GetActionByName(const std::string& name) const;
		void Enable();
		void Disable();
		bool Contains(InputAction* action) const;
	};
}