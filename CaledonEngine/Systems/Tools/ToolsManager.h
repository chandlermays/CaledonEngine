/*------------------------------
| File: ToolsManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Engine/IRenderable.h"
#include "Systems/Engine/IUpdatable.h"
#include "Systems/Tools/DebugOverlay.h"

namespace CE
{
	class InputManager;

	class ToolsManager : public Manager, public IRenderable, public IUpdatable
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
		void Update(float) override;
		void Render() override;
		void Shutdown() override;

		DebugOverlay& GetDebugOverlay() { return m_debugOverlay; }
	};
}