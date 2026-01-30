#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include "CaledonEngine/Systems/Tools/DebugOverlay.h"

namespace CE
{
	class InputManager;

	class ToolsManager : public Manager
	{
	private:
		InputManager* m_pInputManager;
		DebugOverlay m_debugOverlay;

	public:
		ToolsManager();
		~ToolsManager();
		ToolsManager(const ToolsManager&) = delete;
		ToolsManager& operator=(const ToolsManager&) = delete;
		ToolsManager(ToolsManager&&) = delete;
		ToolsManager& operator=(ToolsManager&&) = delete;

		bool Initialize() override;
		void Render() override;
		void Update(float) override;
		void Shutdown() override;
	};
}