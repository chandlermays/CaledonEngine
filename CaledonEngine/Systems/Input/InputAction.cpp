#include "InputAction.h"

CE::InputBinding::InputBinding(KeyCode key)
    : m_keyCode{ key }
	, m_mouseCode{ MouseCode::kNone }
	, m_isMouseButton{ false }
{}

CE::InputBinding::InputBinding(MouseCode mouse)
    : m_keyCode{ KeyCode::kNone }
	, m_mouseCode{ mouse }
	, m_isMouseButton{ true }
{}

CE::InputAction::InputAction(const std::string& name, ActionType type)
    : m_name{ name }
    , m_actionType{ type }
    , m_controlType{ ControlType::kAny }
	, m_initialStateCheck{ false }
	, m_isEnabled{ true }
{}

void CE::InputAction::SetActionType(ActionType type)
{
    m_actionType = type;
}

void CE::InputAction::SetControlType(ControlType type)
{
    m_controlType = type;
}

void CE::InputAction::SetInitialStateCheck(bool flag)
{
	m_initialStateCheck = flag;
}

void CE::InputAction::AddBinding(KeyCode key)
{
    m_inputBindings.emplace_back(key);
}

void CE::InputAction::AddBinding(MouseCode mouse)
{
    m_inputBindings.emplace_back(mouse);
}

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

void CE::InputAction::ClearBindings()
{
    m_inputBindings.clear();
	m_compositeBindings.clear();
}

void CE::InputAction::AddPositiveBinding(KeyCode key)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_positiveBindings.emplace_back(key);
}

void CE::InputAction::AddPositiveBinding(MouseCode mouse)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_positiveBindings.emplace_back(mouse);
}

void CE::InputAction::AddNegativeBinding(KeyCode key)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_negativeBindings.emplace_back(key);
}

void CE::InputAction::AddNegativeBinding(MouseCode mouse)
{
	if (m_compositeBindings.empty())
	{
		m_compositeBindings.emplace_back();
	}
	m_compositeBindings[0].m_negativeBindings.emplace_back(mouse);
}

void CE::InputAction::OnStarted(std::function<void()> callback)
{
    m_onStartedCallbacks.emplace_back(callback);
}

void CE::InputAction::OnPerformed(std::function<void()> callback)
{
    m_onPerformedCallbacks.emplace_back(callback);
}

void CE::InputAction::OnCanceled(std::function<void()> callback)
{
    m_onCanceledCallbacks.emplace_back(callback);
}

void CE::InputAction::OnValue(std::function<void(float)> callback)
{
    m_onValueCallbacks.emplace_back(callback);
}

void CE::InputAction::InvokeStartedCallbacks()
{
    for (const auto& callback : m_onStartedCallbacks)
    {
        callback();
    }
}

void CE::InputAction::InvokePerformedCallbacks()
{
    for (const auto& callback : m_onPerformedCallbacks)
    {
        callback();
	}
}

void CE::InputAction::InvokeCanceledCallbacks()
{
    for (const auto& callback : m_onCanceledCallbacks)
    {
        callback();
	}
}

void CE::InputAction::InvokeValueCallbacks(float value)
{
    for (const auto& callback : m_onValueCallbacks)
    {
        callback(value);
	}
}

const std::string& CE::InputAction::GetName() const
{
    return m_name;
}

CE::ActionType CE::InputAction::GetActionType() const
{
    return m_actionType;
}

CE::ControlType CE::InputAction::GetControlType() const
{
    return m_controlType;
}

bool CE::InputAction::GetInitialStateCheck() const
{
    return m_initialStateCheck;
}

bool CE::InputAction::IsEnabled() const
{
    return m_isEnabled;
}

const std::vector<CE::InputBinding>& CE::InputAction::GetInputBindings() const
{
	return m_inputBindings;
}

const std::vector<CE::CompositeBinding>& CE::InputAction::GetCompositeBindings() const
{
	return m_compositeBindings;
}
