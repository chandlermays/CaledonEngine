#pragma once
#include <unordered_map>

namespace CE
{
	enum class KeyCode : uint8_t
	{
		kNone,
		kUp,
		kDown,
		kLeft,
		kRight,
		kInteract,
		kEscape,
		kPageUp,
		kPageDown,
		kBackspace,
		kEnter,
		kSpace,
		kLeftShift,
		kRightShift,
		kLeftCtrl,
		kRightCtrl,
		kLeftAlt,
		kRightAlt,
		kTab,
		kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL, kM, 
		kN, kO, kP, kQ, kR, kS, kT, kU, kV, kW, kX, kY, kZ,
		k1, k2, k3, k4, k5, k6, k7, k8, k9, k0,
		kF1, kF2, kF3, kF4, kF5, kF6, kF7, kF8, kF9, kF10, kF11, kF12,
		kHome,
		kEnd,
		kInsert,
		kDelete,
		kCapsLock,
		kNumLock,
		kScrollLock,
		kPrintScreen,
		kPlus,
		kMinus,
		kMultiply,
		kDivide,
		kEquals,
		kLeftBracket,
		kRightBracket,
		kSemicolon,
		kApostrophe,
		kComma,
		kPeriod,
		kSlash,
		kBackslash,
		kTilde,
		kNumKeys
	};

	enum class MouseCode : uint8_t
	{
		kNone,
		kMouseLeft,
		kMouseRight,
		kMouseMiddle,
		kMouse4,
		kMouse5,
		kNumMouseButtons
	};

	class Input
	{
	protected:
		std::unordered_map<KeyCode, bool> m_keyStates;						// State of the Key: Pressed vs. Released
		std::unordered_map<KeyCode, bool> m_prevKeyStates;					// State of the Key in the previous frame
		std::unordered_map<MouseCode, bool> m_mouseStates;					// State of the Mouse Button: Pressed vs. Released
		std::unordered_map<MouseCode, bool> m_prevMouseStates;				// State of the Mouse Button in the previous frame

		int m_mousePositionX;												// Current Mouse X Position
		int m_mousePositionY;												// Current Mouse Y Position
		int m_mouseWheelDelta;												// Mouse Wheel Delta since last frame

	public:
		Input();															// Constructor
		virtual ~Input() = default;											// Destructor
		Input(const Input&) = delete;										// Prevent copy-construction
		Input& operator=(const Input&) = delete;							// Prevent copy-assignment
		Input(Input&&) = delete;											// Prevent move-construction
		Input& operator=(Input&&) = delete;									// Prevent move-assignment

		virtual bool Initialize() = 0;										// Initialize the Input States
		virtual void Shutdown() = 0;										// Shutdown the Input System
		virtual bool ProcessEvents() = 0;									// Process Input Events

		virtual bool IsKeyHeld(KeyCode key) const;							// Returns true if the specified key is currently held down
		virtual bool IsKeyPressed(KeyCode key) const;						// Returns true if the specified key was pressed this frame
		virtual bool IsKeyReleased(KeyCode key) const;						// Returns true if the specified key was released this frame

		virtual bool IsMouseButtonHeld(MouseCode button) const;				// Returns true if the specified mouse button is currently held down
		virtual bool IsMouseButtonPressed(MouseCode button) const;			// Returns true if the specified mouse button was pressed this frame
		virtual bool IsMouseButtonReleased(MouseCode button) const;			// Returns true if the specified mouse button was released this frame

		virtual void GetMousePosition(int& x, int& y) const;				// Retrieves the current mouse position
		virtual int GetMouseWheelDelta() const;								// Retrieves the mouse wheel delta since last frame
	};
}