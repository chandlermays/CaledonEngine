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
    return false;
}

bool CE::Input::IsKeyPressed(KeyCode key) const
{
    return false;
}

bool CE::Input::IsKeyReleased(KeyCode key) const
{
    return false;
}

bool CE::Input::IsMouseButtonHeld(MouseCode button) const
{
    return false;
}

bool CE::Input::IsMouseButtonPressed(MouseCode button) const
{
    return false;
}

bool CE::Input::IsMouseButtonReleased(MouseCode button) const
{
    return false;
}

void CE::Input::GetMousePosition(int& x, int& y) const
{
}

int CE::Input::GetMouseWheelDelta() const
{
    return 0;
}
