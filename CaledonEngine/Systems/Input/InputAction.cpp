/*------------------------------
| File: InputAction.cpp
| Author: Chandler Mays
------------------------------*/
#include "InputAction.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------------
| --- Constructor: Constructs a Keyboard Input Binding with default values --- |
------------------------------------------------------------------------------*/
CE::InputBinding::InputBinding(KeyCode key)
    : m_keyCode{ key }
	, m_mouseCode{ MouseCode::kNone }
	, m_isMouseButton{ false }
{}

/*----------------------------------------------------------------------------------
| --- Constructor: Constructs a Mouse Button Input Binding with default values --- |
----------------------------------------------------------------------------------*/
CE::InputBinding::InputBinding(MouseCode mouse)
    : m_keyCode{ KeyCode::kNone }
	, m_mouseCode{ mouse }
	, m_isMouseButton{ true }
{}

/*--------------------------------------------------------------------
| --- Constructor: Constructs an InputAction with default values --- |
--------------------------------------------------------------------*/
CE::InputAction::InputAction(const std::string& name, ActionType type)
    : m_name{ name }
    , m_actionType{ type }
    , m_controlType{ ControlType::kAny }
	, m_initialStateCheck{ false }
	, m_isEnabled{ true }
{}

/*----------------------------------------------------------------
| --- SetActionType: Sets the action type of an input action --- |
----------------------------------------------------------------*/
void CE::InputAction::SetActionType(ActionType type)
{
    m_actionType = type;
}

/*------------------------------------------------------------------
| --- SetControlType: Sets the control type of an input action --- |
------------------------------------------------------------------*/
void CE::InputAction::SetControlType(ControlType type)
{
    m_controlType = type;
}

/*-----------------------------------------------------------------
| --- SetInitialStateCheck: Sets the initial state check flag --- |
-----------------------------------------------------------------*/
void CE::InputAction::SetInitialStateCheck(bool flag)
{
	m_initialStateCheck = flag;
}

/*----------------------------------------------------------------------
| --- AddBinding: Adds an input binding to a keyboard input action --- |
----------------------------------------------------------------------*/
void CE::InputAction::AddBinding(KeyCode key)
{
    m_inputBindings.emplace_back(key);
}

/*-------------------------------------------------------------------
| --- AddBinding: Adds an input binding to a mouse input action --- |
-------------------------------------------------------------------*/
void CE::InputAction::AddBinding(MouseCode mouse)
{
    m_inputBindings.emplace_back(mouse);
}

/*------------------------------------------------------------------------------
| --- RemoveBinding: Removes an input binding from a keyboard input action --- |
------------------------------------------------------------------------------*/
void CE::InputAction::RemoveBinding(KeyCode key)
{
    m_inputBindings.erase(
        std::remove_if(m_inputBindings.begin(), m_inputBindings.end(),
        [key](const InputBinding& binding)
        {
            return !binding.m_isMouseButton && binding.m_keyCode == key;
        }),
        m_inputBindings.end());
}

/*---------------------------------------------------------------------------
| --- RemoveBinding: Removes an input binding from a mouse input action --- |
---------------------------------------------------------------------------*/
void CE::InputAction::RemoveBinding(MouseCode mouse)
{
    m_inputBindings.erase(
        std::remove_if(m_inputBindings.begin(), m_inputBindings.end(),
        [mouse](const InputBinding& binding)
        {
            return binding.m_isMouseButton && binding.m_mouseCode == mouse;
			}),
        m_inputBindings.end());
}

/*------------------------------------------------------------------
| --- ClearBindings: Clears all input bindings from the action --- |
------------------------------------------------------------------*/
void CE::InputAction::ClearBindings()
{
    m_inputBindings.clear();
	m_compositeBindings.clear();
}

/*--------------------------------------------------------------------------------------------------------
| --- AddPositiveBinding: Adds a positive binding to a composite binding for a keyboard input action --- |
--------------------------------------------------------------------------------------------------------*/
void CE::InputAction::AddPositiveBinding(KeyCode key)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_positiveBindings.emplace_back(key);
}

/*--------------------------------------------------------------------------------------------------------
| --- AddNegativeBinding: Adds a negative binding to a composite binding for a keyboard input action --- |
--------------------------------------------------------------------------------------------------------*/
void CE::InputAction::AddNegativeBinding(KeyCode key)
{
    if (m_compositeBindings.empty())
    {
        m_compositeBindings.emplace_back();
    }
    m_compositeBindings[0].m_negativeBindings.emplace_back(key);
}

/*-----------------------------------------------------------------------------------------------------
| --- AddPositiveBinding: Adds a positive binding to a composite binding for a mouse input action --- |
-----------------------------------------------------------------------------------------------------*/
void CE::InputAction::AddPositiveBinding(MouseCode mouse)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_positiveBindings.emplace_back(mouse);
}

/*-----------------------------------------------------------------------------------------------------
| --- AddNegativeBinding: Adds a negative binding to a composite binding for a mouse input action --- |
-----------------------------------------------------------------------------------------------------*/
void CE::InputAction::AddNegativeBinding(MouseCode mouse)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_negativeBindings.emplace_back(mouse);
}

/*--------------------------------------------------------------------
| --- OnStarted: Registers a callback for when the action starts --- |
--------------------------------------------------------------------*/
void CE::InputAction::OnStarted(std::function<void()> callback)
{
    m_onStartedCallbacks.emplace_back(callback);
}

/*------------------------------------------------------------------------
| --- OnPerformed: Registers a callback for when the action performs --- |
------------------------------------------------------------------------*/
void CE::InputAction::OnPerformed(std::function<void()> callback)
{
    m_onPerformedCallbacks.emplace_back(callback);
}

/*----------------------------------------------------------------------
| --- OnCanceled: Registers a callback for when the action cancels --- |
----------------------------------------------------------------------*/
void CE::InputAction::OnCanceled(std::function<void()> callback)
{
    m_onCanceledCallbacks.emplace_back(callback);
}

/*-------------------------------------------------------------------------
| --- OnValue: Registers a callback for when the action value changes --- |
-------------------------------------------------------------------------*/
void CE::InputAction::OnValue(std::function<void(float)> callback)
{
    m_onValueCallbacks.emplace_back(callback);
}

/*--------------------------------------------------------------------------
| --- InvokeStartedCallbacks: Invokes all registered started callbacks --- |
--------------------------------------------------------------------------*/
void CE::InputAction::InvokeStartedCallbacks()
{
    for (const auto& callback : m_onStartedCallbacks)
    {
        callback();
    }
}

/*------------------------------------------------------------------------------
| --- InvokePerformedCallbacks: Invokes all registered performed callbacks --- |
------------------------------------------------------------------------------*/
void CE::InputAction::InvokePerformedCallbacks()
{
    for (const auto& callback : m_onPerformedCallbacks)
    {
        callback();
	}
}

/*----------------------------------------------------------------------------
| --- InvokeCanceledCallbacks: Invokes all registered canceled callbacks --- |
----------------------------------------------------------------------------*/
void CE::InputAction::InvokeCanceledCallbacks()
{
    for (const auto& callback : m_onCanceledCallbacks)
    {
        callback();
	}
}

/*----------------------------------------------------------------------
| --- InvokeValueCallbacks: Invokes all registered value callbacks --- |
----------------------------------------------------------------------*/
void CE::InputAction::InvokeValueCallbacks(float value)
{
    for (const auto& callback : m_onValueCallbacks)
    {
        callback(value);
	}
}

/*-------------------------------------------------------
| --- GetName: Returns the name of the input action --- |
-------------------------------------------------------*/
const std::string& CE::InputAction::GetName() const
{
    return m_name;
}

/*--------------------------------------------------------------------
| --- GetActionType: Returns the action type of the input action --- |
--------------------------------------------------------------------*/
CE::ActionType CE::InputAction::GetActionType() const
{
    return m_actionType;
}

/*----------------------------------------------------------------------
| --- GetControlType: Returns the control type of the input action --- |
----------------------------------------------------------------------*/
CE::ControlType CE::InputAction::GetControlType() const
{
    return m_controlType;
}

/*--------------------------------------------------------------------
| --- GetInitialStateCheck: Returns the initial state check flag --- |
--------------------------------------------------------------------*/
bool CE::InputAction::GetInitialStateCheck() const
{
    return m_initialStateCheck;
}

/*------------------------------------------------------------------
| --- IsEnabled: Returns the enabled state of the input action --- |
------------------------------------------------------------------*/
bool CE::InputAction::IsEnabled() const
{
    return m_isEnabled;
}

/*--------------------------------------------------------------------
| --- GetInputBindings: Returns the input bindings of the action --- |
--------------------------------------------------------------------*/
const std::vector<CE::InputBinding>& CE::InputAction::GetInputBindings() const
{
	return m_inputBindings;
}

/*----------------------------------------------------------------------------
| --- GetCompositeBindings: Returns the composite bindings of the action --- |
----------------------------------------------------------------------------*/
const std::vector<CE::CompositeBinding>& CE::InputAction::GetCompositeBindings() const
{
	return m_compositeBindings;
}