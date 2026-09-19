#include "EditorSettings.h"

#include <Windows.h>
#include <fstream>

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

void EditorSettings::SetLastProjectPath(const std::string& projectFilePath)
{
	std::ofstream file(GetSettingsFilePath());
	if (file.is_open())
	{
		file << projectFilePath;
	}
}

std::string EditorSettings::GetLastProjectPath()
{
	std::ifstream file(GetSettingsFilePath());
	if (!file.is_open())
		return "";

	std::string path;
	std::getline(file, path);
	return path;
}