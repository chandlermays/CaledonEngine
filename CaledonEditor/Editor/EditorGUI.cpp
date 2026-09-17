/*------------------------------
| File: EditorGUI.cpp
| Author: Chandler Mays
------------------------------*/
#include "EditorGUI.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Rendering/Window.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"

#include <SDL3/SDL.h>
#include <CaledonEngine/lib/ImGUI/imgui.h>
#include <CaledonEngine/lib/ImGUI/imgui_impl_sdl3.h>
#include <CaledonEngine/lib/ImGUI/imgui_impl_sdlrenderer3.h>

EditorGUI::EditorGUI()
	: m_isInitialized{ false }
{ }

EditorGUI::~EditorGUI()
{
	Shutdown();
}

bool EditorGUI::Initialize()
{
    CE::GraphicsManager* graphics = CE::EngineManager::GetInstance().GetGraphicsManager();
    CE::Window* window = graphics ? graphics->GetWindow() : nullptr;
    CE::Renderer* renderer = graphics ? graphics->GetRenderer() : nullptr;

    SDL_Window* sdlWindow = window ? static_cast<SDL_Window*>(window->GetNativeHandle()) : nullptr;
    SDL_Renderer* sdlRenderer = renderer ? static_cast<SDL_Renderer*>(renderer->GetNativeHandle()) : nullptr;

    if (!sdlWindow || !sdlRenderer)
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForSDLRenderer(sdlWindow, sdlRenderer))
    {
        ImGui::DestroyContext();
        return false;
    }

    if (!ImGui_ImplSDLRenderer3_Init(sdlRenderer))
    {
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        return false;
    }

    m_isInitialized = true;
    return true;
}

void EditorGUI::BeginFrame()
{
    if (!m_isInitialized)
        return;

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void EditorGUI::EndFrame()
{
    if (!m_isInitialized)
        return;

    ImGui::Render();

    CE::GraphicsManager* graphics = CE::EngineManager::GetInstance().GetGraphicsManager();
    CE::Renderer* renderer = graphics ? graphics->GetRenderer() : nullptr;
    SDL_Renderer* sdlRenderer = renderer ? static_cast<SDL_Renderer*>(renderer->GetNativeHandle()) : nullptr;

    if (sdlRenderer)
    {
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
    }
}

void EditorGUI::Shutdown()
{
    if (!m_isInitialized)
        return;

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    m_isInitialized = false;
}