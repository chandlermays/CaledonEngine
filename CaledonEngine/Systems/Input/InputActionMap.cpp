/*------------------------------
| File: InputActionMap.cpp
| Author: Chandler Mays
------------------------------*/
#include "InputActionMap.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------
| --- Constructor: Constructs an Input Action Map with default values --- |
-------------------------------------------------------------------------*/
CE::InputActionMap::InputActionMap(const std::string& name)
    : m_name{ name }
	, m_isEnabled{ false }
{}  // By default, maps are disabled and must be enabled by user.

/*----------------------------------------------------
| --- AddAction: Adds an input action to the map --- |
----------------------------------------------------*/
void CE::InputActionMap::AddAction(const std::string& name, ActionType type)
{
	// Should consider checking for duplicates here. We don't want two of the same action!
	m_actions.emplace(name, std::make_unique<InputAction>(name, type));
}

/*------------------------------------------------------------
| --- RemoveAction: Removes an input action from the map --- |
------------------------------------------------------------*/
void CE::InputActionMap::RemoveAction(const std::string& name)
{
	m_actions.erase(name);
}

/*----------------------------------------
| --- Enable: Enables the action map --- |
----------------------------------------*/
void CE::InputActionMap::Enable()
{
    m_isEnabled = true;
}

/*------------------------------------------
| --- Disable: Disables the action map --- |
------------------------------------------*/
void CE::InputActionMap::Disable()
{
	m_isEnabled = false;
}

/*-----------------------------------------------------------------------
| --- Contains: Checks if the action map contains an action by name --- |
-----------------------------------------------------------------------*/
bool CE::InputActionMap::Contains(const std::string& name) const
{
	return m_actions.find(name) != m_actions.end();
}

/*-----------------------------------------------------------
| --- GetInputActions: Returns the map of input actions --- |
-----------------------------------------------------------*/
const std::unordered_map<std::string, std::unique_ptr<CE::InputAction>>& CE::InputActionMap::GetInputActions() const
{
	return m_actions;
}

/*----------------------------------------------------------
| --- GetActionByName: Returns an input action by name --- |
----------------------------------------------------------*/
CE::InputAction* CE::InputActionMap::GetActionByName(const std::string& name) const
{
	auto it = m_actions.find(name);
	return it != m_actions.end() ? it->second.get() : nullptr;
}

/*-----------------------------------------------------
| --- GetName: Returns the name of the action map --- |
-----------------------------------------------------*/
const std::string& CE::InputActionMap::GetName() const
{
	return m_name;
}

/*--------------------------------------------------------
| --- IsEnabled: Checks if the action map is enabled --- |
--------------------------------------------------------*/
bool CE::InputActionMap::IsEnabled() const
{
	return m_isEnabled;
}