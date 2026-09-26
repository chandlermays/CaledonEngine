/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
/*-------------------------------------------------------------------------------------------------
| Entry Point: CaledonEditor
| --------------------------
| Authoring environment and level editor for CaledonEngine projects (similar to Unity Editor).
|
| Responsibilities:
| - Serves as a Visual Studio startup project during gameplay design, level authoring, and debugging.
| - Loads project configurations, scenes, and dynamic gameplay assemblies.
| - Manages ImGui editor tooling, docking panels, inspector views, and the live Viewport.
| - Shares the core runtime with the standalone player by linking CaledonEngine.lib statically.
-------------------------------------------------------------------------------------------------*/
#include "Editor.h"

#include <string>

int main(int argc, char* argv[])
{
	Editor editor;

	std::string projectPath = (argc > 1) ? argv[1] : "";

	if (!editor.Initialize(projectPath))
		return -1;

	editor.Run();

	return 0;
}