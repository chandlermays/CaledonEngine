#pragma once
#include <string>

namespace EditorSettings
{
	void SetLastProjectPath(const std::string& projectFilePath);
	std::string GetLastProjectPath();
}