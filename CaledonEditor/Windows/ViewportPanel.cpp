/*------------------------------
| File: ViewportPanel.cpp
| Author: Chandler Mays
------------------------------*/
#include "ViewportPanel.h"
#include "Editor/EditorContext.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Systems/Rendering/Texture.h"
#include "CaledonEngine/Systems/Scene/SceneManager.h"

#include <CaledonEngine/lib/ImGUI/imgui.h>

ViewportPanel::ViewportPanel()
	: m_width{ 0 }
	, m_height{ 0 }
{
}

void ViewportPanel::EnsureRenderTexture(int width, int height, CE::SceneManager* pSceneManager)
{
	if (width == m_width && height == m_height && m_pRenderTexture)
		return;

	CE::GraphicsManager* pGraphicsManager = CE::EngineManager::GetInstance().GetGraphicsManager();
	CE::Renderer* pRenderer = pGraphicsManager ? pGraphicsManager->GetRenderer() : nullptr;
	if (!pRenderer)
		return;

	m_pRenderTexture = pRenderer->CreateRenderTarget(width, height);
	m_width = width;
	m_height = height;

	if (pSceneManager)
	{
		pSceneManager->SetRenderTarget(m_pRenderTexture.get());
	}
}

void ViewportPanel::Draw(EditorContext&)
{
	CE::SceneManager* pSceneManager = CE::EngineManager::GetInstance().GetSceneManager();

	ImGui::Begin("Viewport");

	if (!pSceneManager || !pSceneManager->GetCurrentScene())
	{
		ImGui::Text("No active scene.");
		ImGui::End();
		return;
	}

	ImVec2 available = ImGui::GetContentRegionAvail();
	int width = static_cast<int>(available.x);
	int height = static_cast<int>(available.y);

	if (width > 0 && height > 0)
	{
		EnsureRenderTexture(width, height, pSceneManager);

		if (m_pRenderTexture)
		{
			ImTextureID textureId = reinterpret_cast<ImTextureID>(m_pRenderTexture->GetNativeHandle());
			ImGui::Image(textureId, ImVec2(static_cast<float>(width), static_cast<float>(height)));
		}
	}

	ImGui::End();
}