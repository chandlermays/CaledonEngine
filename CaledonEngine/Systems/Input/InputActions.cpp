#include "InputActions.h"

CE::InputActions::InputActions(const std::string& assetName)
	: m_assetName{ assetName }
	, m_isEnabled{ true }
{}

void CE::InputActions::AddActionMap(const std::string& name)
{
	// Should consider checking for duplicates here. We don't want two of the same action map!
	CE::InputActionMap* pActionMap = new CE::InputActionMap(name);
	m_actionMaps.emplace(name, pActionMap);
}

void CE::InputActions::RemoveActionMap(const std::string& name)
{
	auto it = m_actionMaps.find(name);
	if (it == m_actionMaps.end())
		return;

	delete it->second;
	m_actionMaps.erase(it);
}

const CE::InputActionMap* CE::InputActions::GetActionMapByName(const std::string& name) const
{
	auto it = m_actionMaps.find(name);
	if (it != m_actionMaps.end())
	{
		return it->second;
	}
	return nullptr;
}

const std::unordered_map<std::string, CE::InputActionMap*>& CE::InputActions::GetInputActionMaps() const
{
	return m_actionMaps;
}

bool CE::InputActions::IsEnabled() const
{
	return m_isEnabled;
}

CE::InputAction* CE::InputActions::GetActionByName(const std::string& name) const
{
	for (const auto& [mapName, actionMap] : m_actionMaps)
	{
		InputAction* pAction = actionMap->GetActionByName(name);
		if (pAction != nullptr)
		{
			return pAction;
		}
	}
	return nullptr;
}

void CE::InputActions::Enable()
{
	m_isEnabled = true;
}

void CE::InputActions::Disable()
{
	m_isEnabled = false;
}

bool CE::InputActions::Contains(InputAction* action) const
{
	return false;
}