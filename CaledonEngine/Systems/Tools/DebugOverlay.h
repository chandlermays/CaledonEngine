/*------------------------------
| File: DebugOverlap.h
| Author: Chandler Mays
------------------------------*/
#pragma once

namespace CE
{
	class DebugOverlay
	{
	private:
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

		void ToggleVisibility();
		bool IsVisible() const;
	};
}