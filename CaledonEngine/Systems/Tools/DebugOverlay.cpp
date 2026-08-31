/*------------------------------
| File: DebugOverlap.cpp
| Author: Chandler Mays
------------------------------*/
#include "DebugOverlay.h"

#include <ImGUI/imgui.h>

CE::DebugOverlay::DebugOverlay()
	: m_isVisible{ false }
{ }

bool CE::DebugOverlay::Initialize()
{
	// Initialize ImGUI files and "context" here...

	return true;
}

void CE::DebugOverlay::Render()
{
	if (!m_isVisible)
		return;

	// Display ImGui Menu + tabs here...
}

void CE::DebugOverlay::Shutdown()
{
	// ImGUI close methods?
}

void CE::DebugOverlay::ToggleVisibility()
{
	m_isVisible = !m_isVisible;
}

bool CE::DebugOverlay::IsVisible() const
{
	return m_isVisible;
}