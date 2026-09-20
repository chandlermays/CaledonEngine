#include "FileDialog.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Rendering/Window.h"

#include <Windows.h>
#include <Commdlg.h>
#include <SDL3/SDL.h>

namespace
{
	HWND GetOwnerWindowHandle()
	{
		CE::GraphicsManager* pGraphicsManager = CE::EngineManager::GetInstance().GetGraphicsManager();
		CE::Window* pWindow = pGraphicsManager ? pGraphicsManager->GetWindow() : nullptr;
		SDL_Window* pSDLWindow = pWindow ? static_cast<SDL_Window*>(pWindow->GetNativeHandle()) : nullptr;
		if (!pSDLWindow)
			return nullptr;

		SDL_PropertiesID properties = SDL_GetWindowProperties(pSDLWindow);
		return static_cast<HWND>(SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
	}
}

std::string FileDialog::BrowseForProjectFile()
{
	char pathBuffer[512] = {};

	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetOwnerWindowHandle();
	ofn.lpstrFilter = "Caledon Project (*.ceproj)\0*.ceproj\0\0";
	ofn.lpstrFile = pathBuffer;
	ofn.nMaxFile = sizeof(pathBuffer);
	ofn.lpstrTitle = "Open Caledon Project";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn))
	{
		return std::string(pathBuffer);
	}

	return "";
}