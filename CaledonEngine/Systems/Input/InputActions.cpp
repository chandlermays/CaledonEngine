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
	CE::InputActionMap* pActionMap = new CE::InputActionMap(name);
	m_actionMaps.emplace(name, pActionMap);
}

/*---------------------------------------------------------------------
| --- RemoveActionMap: Removes an input action map from the asset --- |
---------------------------------------------------------------------*/
void CE::InputActions::RemoveActionMap(const std::string& name)
{
	auto it = m_actionMaps.find(name);
	if (it == m_actionMaps.end())
		return;

	delete it->second;
	m_actionMaps.erase(it);
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
	return false;
}

/*------------------------------------------------------------------
| --- GetInputActionMaps: Returns the map of input action maps --- |
------------------------------------------------------------------*/
const std::unordered_map<std::string, CE::InputActionMap*>& CE::InputActions::GetInputActionMaps() const
{
	return m_actionMaps;
}

/*--------------------------------------------------------------
| --- GetActionByName: Returns an input action map by name --- |
--------------------------------------------------------------*/
const CE::InputActionMap* CE::InputActions::GetActionMapByName(const std::string& name) const
{
	auto it = m_actionMaps.find(name);
	if (it != m_actionMaps.end())
	{
		return it->second;
	}
	return nullptr;
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