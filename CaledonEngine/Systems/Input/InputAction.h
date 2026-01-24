#pragma once
#include "Input.h"
#include <string>
#include <functional>

// This class represents the individual input actions and their action properties (action type, interactions, processors, etc.)

// An InputAction has three types...
// 1. Action Type: A Value action will start and perform when a control moves from its default value and then perform on every value change.
// It will cancel when controls go back to default value. Also, when enabled, a Value action will respond right away to a control's current value.
// It has a Control Type (any, axis, analog, integer, ...)

// 2. Action Type: A Button action will start when a button is pressed and perform when the press threshold is reached. It will cancel when the button is
// going below the release threshold. Also, if a button is already pressed when the action is enabled, the button has to be released first.
// It has an Initial State Check flag (what this does is check controls bound to the action and if they are already actuated (ie. at non-default value),
// the action will immediately be started and performed.)

// 3. Action Type: A Pass-Through action will not explicitly start and will never cancel. Instead, for every value change on any bound control, the action will perform.
// It has a Control Type (any, axis, analog, integer, ...) AND an Initial State Check flag.

// Methods to add...
// AddInteraction() - adds an interaction to the action (eg. Hold, Tap, etc.).

namespace CE
{
	enum class ActionType
	{
		kValue,
		kButton,
		//...
	};

	enum class ControlType
	{
		kAny,
		kAxis,
		kVector2
		//...
	};

	struct InputBinding
	{
		KeyCode m_keyCode;
		MouseCode m_mouseCode;
		bool m_isMouseButton;

		InputBinding(KeyCode key);
		InputBinding(MouseCode mouse);
	};

	struct CompositeBinding
	{
		std::vector<InputBinding> m_positiveBindings;		// Bindings for positive direction
		std::vector<InputBinding> m_negativeBindings;		// Bindings for negative direction
	};

	class InputAction
	{
	private:
		std::string m_name;
		ActionType m_actionType;
		ControlType m_controlType;
		bool m_initialStateCheck;
		bool m_isEnabled;
		
		std::vector<InputBinding> m_inputBindings;
		std::vector<CompositeBinding> m_compositeBindings;

		std::vector<std::function<void()>> m_onStartedCallbacks;
		std::vector<std::function<void()>> m_onPerformedCallbacks;
		std::vector<std::function<void()>> m_onCanceledCallbacks;
		std::vector<std::function<void(float)>> m_onValueCallbacks;

	public:
		InputAction(const std::string& name, ActionType type = ActionType::kButton);
		~InputAction() = default;

		void SetActionType(ActionType type);
		void SetControlType(ControlType type);
		void SetInitialStateCheck(bool flag);

		void AddBinding(KeyCode key);
		void AddBinding(MouseCode mouse);
		void RemoveBinding(KeyCode key);
		void RemoveBinding(MouseCode mouse);
		void ClearBindings();

		void AddPositiveBinding(KeyCode key);
		void AddPositiveBinding(MouseCode mouse);
		void AddNegativeBinding(KeyCode key);
		void AddNegativeBinding(MouseCode mouse);

		void OnStarted(std::function<void()> callback);
		void OnPerformed(std::function<void()> callback);
		void OnCanceled(std::function<void()> callback);
		void OnValue(std::function<void(float)> callback);

		void InvokeStartedCallbacks();
		void InvokePerformedCallbacks();
		void InvokeCanceledCallbacks();
		void InvokeValueCallbacks(float value);

		const std::string& GetName() const;
		ActionType GetActionType() const;
		ControlType GetControlType() const;
		bool GetInitialStateCheck() const;
		bool IsEnabled() const;
		const std::vector<InputBinding>& GetInputBindings() const;
		const std::vector<CompositeBinding>& GetCompositeBindings() const;
	};
}