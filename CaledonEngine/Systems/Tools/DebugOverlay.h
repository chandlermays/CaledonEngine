/*------------------------------
| File: DebugOverlap.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <functional>
#include <vector>

// TODO: Rename this class to something more intuitive. This acts as the connection (or middle-man)
// between the engine and the editor to display ImGUI panels for the CaledonEditor user interface.
// Should this even be in CaledonEngine? Maybe it should be in CaledonEditor instead...
// 
// DebugOverlay's original intention was to be a debug overlay for the engine to display engine-related information,
// but it has since been repurposed to be a more general-purpose ImGUI panel manager for the editor.

namespace CE
{
	class DebugOverlay
	{
	private:
		std::vector<std::function<void()>> m_panelCallbacks;
		bool m_isVisible;

	public:
		DebugOverlay();
		~DebugOverlay() = default;
		DebugOverlay(const DebugOverlay&) = delete;
		DebugOverlay& operator=(const DebugOverlay&) = delete;
		DebugOverlay(DebugOverlay&&) = delete;
		DebugOverlay& operator=(DebugOverlay&&) = delete;

		bool Initialize();
		void Render();
		void Shutdown();

		void AddPanel(std::function<void()> drawCallback);
		void SetVisible(bool isVisible);
		void ToggleVisibility();
		bool IsVisible() const;
	};
}