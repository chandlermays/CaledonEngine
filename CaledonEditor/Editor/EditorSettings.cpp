#include "EditorSettings.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <Windows.h>

namespace
{
	std::string GetSettingsFilePath()
	{
		char buffer[MAX_PATH];
		DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
		std::string exePath = (length > 0) ? std::string(buffer, length) : ".";
		size_t lastSlash = exePath.find_last_of("\\/");
		std::string exeDirectory = (lastSlash != std::string::npos) ? exePath.substr(0, lastSlash) : ".";
		return exeDirectory + "\\EditorSettings.txt";
	}
}

constexpr size_t kMaxRecentProjects = 10;

std::vector<std::string> ReadAllPaths()
{
	std::vector<std::string> paths;
	std::ifstream file(GetSettingsFilePath());
	for (std::string line; std::getline(file, line); )
	{
		if (!line.empty())
			paths.push_back(line);
	}
	return paths;
}

std::vector<std::string> EditorSettings::GetRecentProjects()
{
	std::vector<std::string> paths = ReadAllPaths();
	std::erase_if(paths, [](const std::string& p) { std::error_code ec; return !std::filesystem::exists(p, ec); });
	return paths;
}

void EditorSettings::AddRecentProject(const std::string& projectFilePath)
{
	std::vector<std::string> paths = ReadAllPaths();
	std::erase_if(paths, [&](const std::string& p) { return _stricmp(p.c_str(), projectFilePath.c_str()) == 0; });
	paths.insert(paths.begin(), projectFilePath);
	if (paths.size() > kMaxRecentProjects)
		paths.resize(kMaxRecentProjects);

	std::ofstream file(GetSettingsFilePath());
	for (const std::string& p : paths)
		file << p << '\n';
}

std::string EditorSettings::GetLastProjectPath()
{
	const std::vector<std::string> recents = GetRecentProjects();
	return recents.empty() ? "" : recents.front();
}