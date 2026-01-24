#include "Input.h"

/*-------------------------------------
| --- Public Function Definitions --- |
-------------------------------------*/
/*---------------------------------------------------------------
| --- Constructor: Constructs the Input with default values --- |
---------------------------------------------------------------*/
CE::Input::Input()
    : m_mousePositionX{ 0 }
    , m_mousePositionY{ 0 }
	, m_mouseWheelDelta{ 0 }
{
	// Initialize all keys to the released state
    for (int i = 0; i < static_cast<int>(KeyCode::kNumKeys); ++i)
    {
        m_keyStates[static_cast<KeyCode>(i)] = false;
		m_prevKeyStates[static_cast<KeyCode>(i)] = false;
    }

	// Initialize all mouse buttons to the released state
    for (int i = 0; i < static_cast<int>(MouseCode::kNumMouseButtons); ++i)
    {
        m_mouseStates[static_cast<MouseCode>(i)] = false;
        m_prevMouseStates[static_cast<MouseCode>(i)] = false;
	}
}

bool CE::Input::IsKeyHeld(KeyCode key) const
{
	auto it = m_keyStates.find(key);
    if (it != m_keyStates.end())
    {
        return it->second;
	}
	return false;
}

bool CE::Input::IsKeyPressed(KeyCode key) const
{
	auto currentIt = m_keyStates.find(key);
	auto prevIt = m_prevKeyStates.find(key);

    if (currentIt != m_keyStates.end() && prevIt != m_prevKeyStates.end())
    {
        return (currentIt->second && !prevIt->second);
	}
	return false;
}

bool CE::Input::IsKeyReleased(KeyCode key) const
{
    auto currentIt = m_keyStates.find(key);
    auto prevIt = m_prevKeyStates.find(key);

    if (currentIt != m_keyStates.end() && prevIt != m_prevKeyStates.end())
    {
        return (!currentIt->second && prevIt->second);
    }
	return false;
}

bool CE::Input::IsMouseButtonHeld(MouseCode button) const
{
	auto it = m_mouseStates.find(button);
    if (it != m_mouseStates.end())
    {
        return it->second;
	}
	return false;
}

bool CE::Input::IsMouseButtonPressed(MouseCode button) const
{
	auto currentIt = m_mouseStates.find(button);
    auto prevIt = m_prevMouseStates.find(button);

    if (currentIt != m_mouseStates.end() && prevIt != m_prevMouseStates.end())
    {
        return (currentIt->second && !prevIt->second);
	}
	return false;
}

bool CE::Input::IsMouseButtonReleased(MouseCode button) const
{
    auto currentIt = m_mouseStates.find(button);
    auto prevIt = m_prevMouseStates.find(button);

    if (currentIt != m_mouseStates.end() && prevIt != m_prevMouseStates.end())
    {
        return (!currentIt->second && prevIt->second);
    }
	return false;
}

void CE::Input::GetMousePosition(int& x, int& y) const
{
    x = m_mousePositionX;
	y = m_mousePositionY;
}

int CE::Input::GetMouseWheelDelta() const
{
	return m_mouseWheelDelta;
}
