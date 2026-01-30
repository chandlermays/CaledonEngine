#include "ToolsManager.h"
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
	assert(m_pInputManager);

	return m_debugOverlay.Initialize();
}

void CE::ToolsManager::Render()
{
	m_debugOverlay.Render();
}

void CE::ToolsManager::Update(float)
{
	auto* pInput = m_pInputManager->GetInputAPI();
	assert(pInput);

	if (pInput->IsKeyHeld(KeyCode::kLeftShift) && pInput->IsKeyPressed(KeyCode::kTilde))
	{
		m_debugOverlay.ToggleVisibility();
	}
}

void CE::ToolsManager::Shutdown()
{
	m_debugOverlay.Shutdown();
}