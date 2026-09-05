/*------------------------------
| File: DebugOverlap.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <functional>
#include <vector>

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