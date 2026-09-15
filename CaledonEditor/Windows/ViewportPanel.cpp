/*------------------------------
| File: ViewportPanel.cpp
| Author: Chandler Mays
------------------------------*/
#include "ViewportPanel.h"
#include "Editor/EditorContext.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"
#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/Systems/Rendering/Texture.h"
#include "CaledonEngine/API/SDL/SDLTexture.h"

#include <SDL3/SDL.h>
#include <CaledonEngine/lib/ImGUI/imgui.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the ViewportPanel with default values --- |
-----------------------------------------------------------------------*/
ViewportPanel::ViewportPanel()
	: m_pRenderTarget{ nullptr }
	, m_viewportWidth{ 1280 }
	, m_viewportHeight{ 720 }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
ViewportPanel::~ViewportPanel()
{
	Shutdown();
}

/*-----------------------------------------------------------
| --- Initialize: Initializes the ViewportPanel for use --- |
-----------------------------------------------------------*/
bool ViewportPanel::Initialize()
{
	// Create the off-screen render target with default dimensions
	if (!CreateRenderTarget(m_viewportWidth, m_viewportHeight))
	{
		return false;
	}

	return true;
}

/*--------------------------------------------------------------------------
| --- Draw: Draws the Viewport panel with the currently rendered scene --- |
--------------------------------------------------------------------------*/
void ViewportPanel::Draw(EditorContext& context)
{
	CE::EngineManager& engineManager = CE::EngineManager::GetInstance();
	CE::GraphicsManager* pGraphicsManager = engineManager.GetGraphicsManager();
	CE::SceneManager* pSceneManager = engineManager.GetSceneManager();
	CE::Renderer* pRenderer = pGraphicsManager ? pGraphicsManager->GetRenderer() : nullptr;
	CE::Scene* pScene = pSceneManager ? pSceneManager->GetCurrentScene() : nullptr;
	SDL_Renderer* pSDLRenderer = pRenderer ? static_cast<SDL_Renderer*>(pRenderer->GetNativeHandle()) : nullptr;

	if (!pSDLRenderer)
	{
		ImGui::Begin("Viewport");
		ImGui::Text("Renderer not available.");
		ImGui::End();
		return;
	}

	ImGui::Begin("Viewport");

	if (!pScene)
	{
		ImGui::Text("No active scene.");
		ImGui::End();
		return;
	}

	// Get the available content region size for the viewport panel
	ImVec2 panelSize = ImGui::GetContentRegionAvail();
	if (panelSize.x < 1.0f) panelSize.x = 1.0f;
	if (panelSize.y < 1.0f) panelSize.y = 1.0f;

	int newWidth = static_cast<int>(panelSize.x);
	int newHeight = static_cast<int>(panelSize.y);

	// Recreate render target if size changed
	if (newWidth != m_viewportWidth || newHeight != m_viewportHeight)
	{
		if (!CreateRenderTarget(newWidth, newHeight))
		{
			ImGui::Text("Failed to resize viewport render target.");
			ImGui::End();
			return;
		}
		m_viewportWidth = newWidth;
		m_viewportHeight = newHeight;
	}

	// Render the scene to the render target
	if (m_pRenderTarget)
	{
		// Set the render target for the renderer
		SDL_SetRenderTarget(pSDLRenderer, m_pRenderTarget);

		// Clear and render the scene
		pRenderer->BeginFrame();
		if (pScene)
		{
			pScene->Render();
		}
		pRenderer->EndFrame();

		// Reset render target to default (backbuffer)
		SDL_SetRenderTarget(pSDLRenderer, nullptr);

		// Display the rendered texture in ImGui
		// Cast SDL_Texture* as ImTextureID (void*) for ImGui::Image()
		ImGui::Image(
			reinterpret_cast<ImTextureID>(m_pRenderTarget),
			ImVec2(static_cast<float>(m_viewportWidth), static_cast<float>(m_viewportHeight)),
			ImVec2(0, 0),
			ImVec2(1, 1)
		);
	}
	else
	{
		ImGui::Text("Render target not initialized.");
	}

	ImGui::End();
}

/*------------------------------------------
| --- Shutdown: Cleans up the viewport --- |
------------------------------------------*/
void ViewportPanel::Shutdown()
{
	DestroyRenderTarget();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-----------------------------------------------------------------
| --- CreateRenderTarget: Creates an off-screen render target --- |
-----------------------------------------------------------------*/
bool ViewportPanel::CreateRenderTarget(int width, int height)
{
	if (width <= 0 || height <= 0)
	{
		return false;
	}

	// Clean up existing render target
	DestroyRenderTarget();

	CE::EngineManager& engineManager = CE::EngineManager::GetInstance();
	CE::GraphicsManager* pGraphicsManager = engineManager.GetGraphicsManager();
	CE::Renderer* pRenderer = pGraphicsManager ? pGraphicsManager->GetRenderer() : nullptr;
	SDL_Renderer* pSDLRenderer = pRenderer ? static_cast<SDL_Renderer*>(pRenderer->GetNativeHandle()) : nullptr;

	if (!pSDLRenderer)
	{
		return false;
	}

	// Create a new SDL_Texture as a render target
	// SDL3 texture flags: SDL_TEXTUREACCESS_TARGET for rendering, SDL_PIXELFORMAT_RGBA8888 for color format
	m_pRenderTarget = SDL_CreateTexture(
		pSDLRenderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width,
		height
	);

	if (!m_pRenderTarget)
	{
		return false;
	}

	// Wrap the SDL texture in a CaledonEngine SDLTexture for API consistency
	auto pNewTexture = std::make_shared<CE::SDLTexture>();
	if (!pNewTexture->Load(m_pRenderTarget, width, height))
	{
		SDL_DestroyTexture(m_pRenderTarget);
		m_pRenderTarget = nullptr;
		return false;
	}

	m_pViewportTexture = pNewTexture;
	return true;
}

/*---------------------------------------------------------
| --- DestroyRenderTarget: Destroys the render target --- |
---------------------------------------------------------*/
void ViewportPanel::DestroyRenderTarget()
{
	if (m_pRenderTarget)
	{
		SDL_DestroyTexture(m_pRenderTarget);
		m_pRenderTarget = nullptr;
	}

	m_pViewportTexture.reset();
}
