/*------------------------------
| File: DebugOverlay.h
| Author: Chandler Mays
------------------------------*/
#pragma once

namespace CE
{
    class DebugOverlay
    {
    private:
		bool m_isVisible;                                                       // Flag to indicate if the debug overlay is visible

    public:
        DebugOverlay();                                                         // Constructor
        ~DebugOverlay() = default;                                              // Destructor
		DebugOverlay(const DebugOverlay&) = delete;                             // Prevent copy-construction
		DebugOverlay& operator=(const DebugOverlay&) = delete;                  // Prevent copy-assignment
		DebugOverlay(DebugOverlay&&) = delete;                                  // Prevent move-construction
		DebugOverlay& operator=(DebugOverlay&&) = delete;                       // Prevent move-assignment

		bool Initialize();                                                      // Initializes the debug overlay for use
		void Draw();                                                            // Draws the debug overlay if it is visible
		void Shutdown();                                                        // Shuts down and cleans up the debug overlay

		void SetVisible(bool isVisible);                                        // Sets the visibility of the debug overlay
		void ToggleVisibility();												// Toggles the visibility of the debug overlay
		bool IsVisible() const;													// Returns whether the debug overlay is currently visible
    };
}