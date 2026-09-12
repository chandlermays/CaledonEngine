#include "CaledonEngine/DynamicLibraryInterface.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"

#include "Controllers/PlayerController.h"
#include "Input/GameInputActions.h"

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

	auto createPlayerController = []() -> CE::Component*
		{
			PlayerController* pController = new PlayerController();
			pController->SetInputActions(s_pActiveInputActions);
			return pController;
		};

	registerFunc("PlayerController", "PacMan",
		[createPlayerController](CE::GameObject*, tinyxml2::XMLElement*) -> CE::Component*
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