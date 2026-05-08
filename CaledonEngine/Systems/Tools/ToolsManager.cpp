#include "ToolsManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Input/InputManager.h"
#include <cassert>

CE::ToolsManager::ToolsManager()
	: m_pInputManager{ nullptr }
{ }

CE::ToolsManager::~ToolsManager()
{
	m_pInputManager = nullptr;
}

bool CE::ToolsManager::Initialize()
{
	m_pInputManager = EngineManager::GetInstance().GetInputManager();
	if (!m_pInputManager)
	{
		CE_LOG("ToolsManager::Initialize - InputManager is null.");
		return false;
	}

	return m_debugOverlay.Initialize();
}

void CE::ToolsManager::Render()
{
	m_debugOverlay.Render();
}

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

void CE::ToolsManager::Shutdown()
{
	m_debugOverlay.Shutdown();
}