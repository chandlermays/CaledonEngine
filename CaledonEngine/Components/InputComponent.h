#pragma once
#include "CaledonEngine/Core/Component.h"

namespace CE
{
	class InputComponent : public Component
	{
	private:


	public:
		InputComponent();
		~InputComponent();
		InputComponent(const InputComponent&) = delete;
		InputComponent& operator=(const InputComponent&) = delete;
		InputComponent(InputComponent&&) = delete;
		InputComponent& operator=(InputComponent&&) = delete;

		bool Initialize() override;
	};
}