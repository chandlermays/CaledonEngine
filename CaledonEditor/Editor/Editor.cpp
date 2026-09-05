#include "Editor.h"
#include "Systems/Engine/EngineManager.h"
#include "Systems/Engine/Manager.h"
#include "Systems/Scene/SceneManager.h"
#include "Systems/Tools/ToolsManager.h"
#include "Core/Scene.h"

Editor::Editor() : m_pEngineManager{ nullptr } {}
Editor::~Editor() { if (m_pEngineManager) m_pEngineManager->Shutdown(); }

bool Editor::Initialize()
{
	m_pEngineManager = &CE::EngineManager::GetInstance();
	if (!m_pEngineManager->Initialize())
		return false;

	CE::ToolsManager* pToolsManager = m_pEngineManager->GetToolsManager();
	if (pToolsManager)
	{
		pToolsManager->GetDebugOverlay().SetVisible(true);
		pToolsManager->GetDebugOverlay().AddPanel([this]() { m_hierarchyPanel.Draw(); });
	}

	CreateEmptyScene();

	return true;
}

void Editor::CreateEmptyScene()
{
	CE::SceneManager* pSceneManager = m_pEngineManager->GetSceneManager();

	auto pScene = std::make_unique<CE::Scene>();
	pScene->SetName("Untitled");

	CE::Scene* pSceneRef = pScene.get();
	pSceneManager->AddScene(std::move(pScene));
	pSceneManager->SetCurrentScene(pSceneRef);
	pSceneRef->Initialize();
}

void Editor::Run()
{
	m_pEngineManager->Run();
}