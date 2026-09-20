/*------------------------------
| File: Main.cpp
| Author: Chandler Mays
------------------------------*/
#include "Editor.h"

#include "CaledonEngine/Utilities/LeakDetector.h"

#include <string>

int main(int argc, char* argv[])
{
	LeakDetector::Start();

	Editor editor;

	std::string projectPath = (argc > 1) ? argv[1] : "";

	if (!editor.Initialize(projectPath))
		return -1;

	editor.Run();

	return 0;
}