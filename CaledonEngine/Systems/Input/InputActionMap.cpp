#include "InputActionMap.h"
#include "InputManager.h"

CE::InputActionMap::InputActionMap(const std::string& name)
    : m_name{ name }
	, m_isEnabled{ false }
{}  // By default, maps are disabled and must be enabled by user.

void CE::InputActionMap::AddAction(const std::string& name, ActionType type)
{
	// Should consider checking for duplicates here. We don't want two of the same action!
	InputAction* pAction = new InputAction(name, type);
	m_actions.emplace(name, pAction);
}

void CE::InputActionMap::RemoveAction(const std::string& name)
{
	auto it = m_actions.find(name);
	if (it == m_actions.end())
		return;

	delete it->second;
	m_actions.erase(it);
}

void CE::InputActionMap::Enable()
{
    m_isEnabled = true;
}

void CE::InputActionMap::Disable()
{
	m_isEnabled = false;
}

bool CE::InputActionMap::Contains(const std::string& name) const
{
	return m_actions.find(name) != m_actions.end();
}

const std::unordered_map<std::string, CE::InputAction*>& CE::InputActionMap::GetInputActions() const
{
	return m_actions;
}

CE::InputAction* CE::InputActionMap::GetActionByName(const std::string& name) const
{
	auto it = m_actions.find(name);
	if (it != m_actions.end())
	{
		return it->second;
	}
	return nullptr;
}

bool CE::InputActionMap::IsEnabled() const
{
	return m_isEnabled;
}
