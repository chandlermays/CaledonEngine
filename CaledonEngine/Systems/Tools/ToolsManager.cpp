/*------------------------------
| File: ToolsManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "ToolsManager.h"

#include "Systems/Engine/LoggingManager.h"
#include "Systems/Engine/EngineManager.h"
#include "Systems/Input/InputManager.h"
#include "Systems/Rendering/GraphicsManager.h"
#include "Systems/Scene/SceneManager.h"

#include <cassert>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------
| --- Constructor: Constructs the ToolsManager with default values --- |
----------------------------------------------------------------------*/
CE::ToolsManager::ToolsManager()
	: m_pInputManager{ nullptr }
	, m_pRenderer{ nullptr }
	, m_pSceneRenderTarget{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::ToolsManager::~ToolsManager()
{
	Shutdown();
}

/*-------------------------------------------------------
| --- Initialize: Prepares the ToolsManager for use --- |
-------------------------------------------------------*/
bool CE::ToolsManager::Initialize()
{
	m_pInputManager = EngineManager::GetInstance().GetInputManager();
	if (!m_pInputManager)
	{
		return false;
	}

	GraphicsManager* pGraphicsMgr = EngineManager::GetInstance().GetGraphicsManager();
	m_pRenderer = pGraphicsMgr ? pGraphicsMgr->GetRenderer() : nullptr;

	return m_debugOverlay.Initialize();
}

/*--------------------------------------------------------------
| --- Update: Checks for input to toggle the debug overlay --- |
--------------------------------------------------------------*/
void CE::ToolsManager::Update(float)
{
	auto* pInput = m_pInputManager->GetInputAPI();
	if (!pInput)
		return;

	if (pInput->IsKeyHeld(KeyCode::kLeftShift) && pInput->IsKeyPressed(KeyCode::kTilde))
	{
		m_debugOverlay.ToggleVisibility();
	}
}

/*-----------------------------------------------------------------------------------
| --- Render: Renders the debug overlay using the current scene's render target --- |
-----------------------------------------------------------------------------------*/
void CE::ToolsManager::Render()
{
	if (!m_debugOverlay.IsVisible())
		return;

	SceneManager* pSceneMgr = EngineManager::GetInstance().GetSceneManager();
	m_pSceneRenderTarget = pSceneMgr ? pSceneMgr->GetRenderTarget() : nullptr;

	if (m_pRenderer && m_pSceneRenderTarget)
	{
		m_pRenderer->SetRenderTarget(m_pSceneRenderTarget);
	}

	m_debugOverlay.Draw();

	if (m_pRenderer && m_pSceneRenderTarget)
	{
		m_pRenderer->SetRenderTarget(nullptr);
	}
}

/*--------------------------------------------------------------------
| --- Shutdown: Shutdown the ToolsManager and clean up resources --- |
--------------------------------------------------------------------*/
void CE::ToolsManager::Shutdown()
{
	m_debugOverlay.Shutdown();

	m_pInputManager = nullptr;
	m_pRenderer = nullptr;
	m_pSceneRenderTarget = nullptr;
}