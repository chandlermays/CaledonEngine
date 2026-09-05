/*------------------------------
| File: DebugOverlay.cpp
| Author: Chandler Mays
------------------------------*/
#include "DebugOverlay.h"

#include "Systems/Engine/EngineManager.h"
#include "Systems/Rendering/GraphicsManager.h"
#include "Systems/Rendering/Window.h"
#include "Systems/Rendering/Renderer.h"

#include <SDL3/SDL.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_sdl3.h>
#include <ImGUI/imgui_impl_sdlrenderer3.h>

CE::DebugOverlay::DebugOverlay()
	: m_isVisible{ false }
{
}

bool CE::DebugOverlay::Initialize()
{
	GraphicsManager* pGraphicsManager = EngineManager::GetInstance().GetGraphicsManager();
	if (!pGraphicsManager)
		return false;

	Window* pWindow = pGraphicsManager->GetWindow();
	Renderer* pRenderer = pGraphicsManager->GetRenderer();
	if (!pWindow || !pRenderer)
		return false;

	SDL_Window* pSDLWindow = static_cast<SDL_Window*>(pWindow->GetNativeHandle());
	SDL_Renderer* pSDLRenderer = static_cast<SDL_Renderer*>(pRenderer->GetNativeHandle());
	if (!pSDLWindow || !pSDLRenderer)
		return false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(pSDLWindow, pSDLRenderer);
	ImGui_ImplSDLRenderer3_Init(pSDLRenderer);

	return true;
}

void CE::DebugOverlay::Render()
{
	if (!m_isVisible)
		return;

	GraphicsManager* pGraphicsManager = EngineManager::GetInstance().GetGraphicsManager();
	Renderer* pRenderer = pGraphicsManager ? pGraphicsManager->GetRenderer() : nullptr;
	SDL_Renderer* pSDLRenderer = pRenderer ? static_cast<SDL_Renderer*>(pRenderer->GetNativeHandle()) : nullptr;
	if (!pSDLRenderer)
		return;

	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	for (const auto& panel : m_panelCallbacks)
	{
		panel();
	}

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), pSDLRenderer);
}

void CE::DebugOverlay::Shutdown()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void CE::DebugOverlay::AddPanel(std::function<void()> drawCallback)
{
	m_panelCallbacks.push_back(std::move(drawCallback));
}

void CE::DebugOverlay::SetVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

void CE::DebugOverlay::ToggleVisibility()
{
	m_isVisible = !m_isVisible;
}

bool CE::DebugOverlay::IsVisible() const
{
	return m_isVisible;
}