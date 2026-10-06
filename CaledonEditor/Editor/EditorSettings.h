/*------------------------------
| File: EditorSettings.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include <string>
#include <vector>

namespace EditorSettings
{
	std::vector<std::string> GetRecentProjects();											//
	void AddRecentProject(const std::string& projectFilePath);								//
	std::string GetLastProjectPath();														//
}