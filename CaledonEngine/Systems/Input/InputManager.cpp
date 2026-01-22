#include "InputManager.h"
#include "CaledonEngine/Utilities/APIConfig.h"

CE::InputManager::InputManager()
	: m_inputAPI{ nullptr }
{}

bool CE::InputManager::Initialize()
{
	m_inputAPI = std::make_unique<CEInput>();
	if (m_inputAPI)
	{
		return m_inputAPI->Initialize();
	}
}

void CE::InputManager::Shutdown()
{
	if (m_inputAPI)
	{
		m_inputAPI->Shutdown();
		m_inputAPI.reset();
	}
}