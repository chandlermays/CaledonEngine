/*------------------------------
| File: ModuleExports.cpp
| Author: Chandler Mays
------------------------------*/
#include "CaledonEngine/DynamicLibraryInterface.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"

#include "Controllers/PlayerController.h"
#include "Input/GameInputActions.h"

#include <memory>

using namespace CE;

namespace
{
	GameInputActions* s_pActiveInputActions = nullptr;
}

/*--------------------------------------------------------------------------------------------------
| --- CreateModuleInputActions: Create and return the GameInputActions instance for the module --- |
--------------------------------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) CE::InputActions* CreateModuleInputActions()
{
	s_pActiveInputActions = new GameInputActions();
	s_pActiveInputActions->Initialize();
	return s_pActiveInputActions;
}

/*----------------------------------------------------------------------------------------------
| --- RegisterModuleComponents: Register the module's components with the ComponentFactory --- |
----------------------------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) void RegisterModuleComponents(CE::ComponentFactory::RegisterFunc registerFunc)
{
	if (!registerFunc)
		return;

	auto createPlayerController = []() -> std::unique_ptr<CE::Component>
		{
			auto pController = std::make_unique<PlayerController>();
			pController->SetInputActions(s_pActiveInputActions);
			return pController;
		};

	registerFunc("PlayerController", "PacMan",
		[createPlayerController](CE::GameObject*, tinyxml2::XMLElement*) -> std::unique_ptr<CE::Component>
		{
			return createPlayerController();
		},
		createPlayerController,
		{
			CE::MakeProperty<PlayerController>("Move Speed",
				[](const PlayerController* p) -> CE::PropertyValue { return p->GetMoveSpeed(); },
				[](PlayerController* p, const CE::PropertyValue& v) { p->SetMoveSpeed(std::get<float>(v)); })
		},
		false);
}

/*------------------------------------------------------------------------------------------------
| --- DestroyModuleInputActions: Deletes the GameInputActions instance created by the module --- |
------------------------------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) void DestroyModuleInputActions(CE::InputActions* pInputActions)
{
	if (pInputActions == s_pActiveInputActions)
	{
		s_pActiveInputActions = nullptr;
	}

	delete pInputActions;
}