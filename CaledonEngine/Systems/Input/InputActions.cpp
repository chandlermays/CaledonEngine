/*------------------------------
| File: InputActions.cpp
| Author: Chandler Mays
------------------------------*/
#include "InputActions.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------------
| --- Constructor: Constructs an InputActions instance with default values --- |
------------------------------------------------------------------------------*/
CE::InputActions::InputActions(const std::string& assetName)
	: m_assetName{ assetName }
	, m_isEnabled{ true }
{}

/*-------------------------------------------------------------
| --- AddActionMap: Adds an input action map to the asset --- |
-------------------------------------------------------------*/
void CE::InputActions::AddActionMap(const std::string& name)
{
	// Should consider checking for duplicates here. We don't want two of the same action map!
	m_actionMaps.emplace(name, std::make_unique<InputActionMap>(name));
}

/*---------------------------------------------------------------------
| --- RemoveActionMap: Removes an input action map from the asset --- |
---------------------------------------------------------------------*/
void CE::InputActions::RemoveActionMap(const std::string& name)
{
	m_actionMaps.erase(name);
}

/*-------------------------------------------
| --- Enable: Enables the input actions --- |
-------------------------------------------*/
void CE::InputActions::Enable()
{
	m_isEnabled = true;
}

/*---------------------------------------------
| --- Disable: Disables the input actions --- |
---------------------------------------------*/
void CE::InputActions::Disable()
{
	m_isEnabled = false;
}

/*-------------------------------------------------------------------------
| --- Contains: Checks if the input actions contain a specific action --- |
-------------------------------------------------------------------------*/
bool CE::InputActions::Contains(InputAction* action) const
{
	if (!action)
		return false;

	for (const auto& [mapName, actionMap] : m_actionMaps)
	{
		if (!actionMap)
			continue;

		const auto& actions = actionMap->GetInputActions();

		for (const auto& [actionName, inputAction] : actions)
		{
			if (inputAction.get() == action)
			{
				return true;
			}
		}
	}

	return false;
}

/*--------------------------------------------------------------------------
| --- GetInputActionMaps: Returns the map of input action maps (const) --- |
--------------------------------------------------------------------------*/
const std::unordered_map<std::string, std::unique_ptr<CE::InputActionMap>>& CE::InputActions::GetInputActionMaps() const
{
	return m_actionMaps;
}

/*-------------------------------------------------------------------------
| --- GetActionMapByName: Returns an input action map by name (const) --- |
-------------------------------------------------------------------------*/
const CE::InputActionMap* CE::InputActions::GetActionMapByName(const std::string& name) const
{
	auto it = m_actionMaps.find(name);
	return it != m_actionMaps.end() ? it->second.get() : nullptr;
}

/*-----------------------------------------------------------------
| --- GetActionMapByName: Returns an input action map by name --- |
-----------------------------------------------------------------*/
CE::InputActionMap* CE::InputActions::GetActionMapByName(const std::string& name)
{
	auto it = m_actionMaps.find(name);
	return it != m_actionMaps.end() ? it->second.get() : nullptr;
}

/*----------------------------------------------------------
| --- GetActionByName: Returns an input action by name --- |
----------------------------------------------------------*/
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

/*------------------------------------------------------------
| --- IsEnabled: Checks if the input actions are enabled --- |
------------------------------------------------------------*/
bool CE::InputActions::IsEnabled() const
{
	return m_isEnabled;
}