#pragma once

namespace CE
{
	class EngineManager;
}

class Editor
{
private:
	CE::EngineManager* m_pEngineManager;
	HierarchyPanel m_hierarchyPanel;

	void CreateEmptyScene();

public:
	Editor();
	~Editor();

	bool Initialize();
	void Run();
};