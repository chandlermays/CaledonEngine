#pragma once
#include "ComponentFactory.h"

namespace CE
{
	class InputActions;
}

namespace CE
{
	namespace DynamicLibraryInterface
	{
		inline constexpr const char* kRegisterComponentsFunctionName = "RegisterModuleComponents";
		using RegisterComponentsFunc = void(*)(ComponentFactory::RegisterFunc);

		inline constexpr const char* kCreateInputActionsFunctionName = "CreateModuleInputActions";
		using CreateInputActionsFunc = InputActions* (*)();
	}
}