/*------------------------------
| File: Project.cpp
| Author: Chandler Mays
------------------------------*/
#include "Project.h"

#include "CaledonEngine/Utilities/CaledonParser.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"

#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"
#include <Windows.h>

std::string Project::s_launchDirectory;

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*---------------------------------------------------------------------------------
| --- CaptureLaunchDirectory:
---------------------------------------------------------------------------------*/
void Project::CaptureLaunchDirectory()
{
	if (!s_launchDirectory.empty())
		return;

	char buffer[MAX_PATH];
	DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);
	s_launchDirectory = (length > 0) ? std::string(buffer, length) : ".";
}

/*--------------------------------------------------------------
| --- Load: Loads the project from the specified file path --- |
--------------------------------------------------------------*/
bool Project::Load(const std::string& projectFilePath)
{
	std::string resolvedFilePath = ResolveAbsolutePath(projectFilePath);

	CE::CaledonParser parser;
	if (!parser.LoadFile(resolvedFilePath))
	{
		CE_LOG("Project::Load - Could not open '{}'", resolvedFilePath);
		return false;
	}

	tinyxml2::XMLElement* pRoot = parser.GetRootElement("CaledonProject");
	if (!pRoot)
	{
		CE_LOG("Project::Load - '{}' is missing the <CaledonProject> root element", resolvedFilePath);
		return false;
	}

	const char* pName = pRoot->Attribute("name");
	m_name = pName ? pName : "Untitled";

	tinyxml2::XMLElement* pModule = pRoot->FirstChildElement("Module");
	if (pModule && pModule->Attribute("path"))
	{
		m_modulePath = pModule->Attribute("path");
	}

	tinyxml2::XMLElement* pMasterAssets = pRoot->FirstChildElement("MasterAssets");
	if (pMasterAssets && pMasterAssets->Attribute("path"))
	{
		m_masterAssetsPath = pMasterAssets->Attribute("path");
	}

	size_t lastSlash = resolvedFilePath.find_last_of("\\/");
	m_rootDirectory = (lastSlash != std::string::npos) ? resolvedFilePath.substr(0, lastSlash) : ".";
	m_projectPath = resolvedFilePath;

	if (!SetCurrentDirectoryA(m_rootDirectory.c_str()))
	{
		CE_LOG("Project::Load - Could not set working directory to '{}'", m_rootDirectory);
		return false;
	}

	return !m_modulePath.empty();
}

/*--------------------------------------------------------------------------------------------
| --- CreateNew: Creates a new project in the specified location with the specified name --- |
--------------------------------------------------------------------------------------------*/
bool Project::CreateNew(const std::string& location, const std::string& projectName)
{
	std::string resolvedLocation = ResolveAbsolutePath(location);

	CreateDirectoryA(resolvedLocation.c_str(), nullptr);

	std::string projectDirectory = resolvedLocation + "\\" + projectName;
	CreateDirectoryA(projectDirectory.c_str(), nullptr);

	std::string assetsDirectory = projectDirectory + "\\Assets";
	CreateDirectoryA(assetsDirectory.c_str(), nullptr);

	std::ofstream masterAssetsFile(assetsDirectory + "\\MasterAssets.xml");
	if (masterAssetsFile.is_open())
	{
		masterAssetsFile << "<MasterAssets>\n</MasterAssets>\n";
		masterAssetsFile.close();
	}

	std::string projectFilePath = projectDirectory + "\\" + projectName + ".ceproj";
	std::ofstream projectFile(projectFilePath);
	if (!projectFile.is_open())
	{
		CE_LOG("Project::CreateNew - Could not create '{}'", projectFilePath);
		return false;
	}

	projectFile << "<CaledonProject name=\"" << projectName << "\">\n";
	projectFile << "\t<Module path=\"" << projectName << "Module.dll\"/>\n";
	projectFile << "\t<MasterAssets path=\"Assets/MasterAssets.xml\"/>\n";
	projectFile << "</CaledonProject>\n";
	projectFile.close();

	return Load(projectFilePath);
}

/*--------------------------------------------------------------------------------------------
| --- RegisterScene:
--------------------------------------------------------------------------------------------*/
bool Project::RegisterScene(const std::string& sceneName, const std::string& scenePath) const
{
	if (m_masterAssetsPath.empty())
	{
		CE_LOG("Project::RegisterScene - No project is loaded; '{}' was not registered", scenePath);
		return false;
	}

	// Paths are relative to the project root, which Load() already made the working directory
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(m_masterAssetsPath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		CE_LOG("Project::RegisterScene - Could not open '{}'", m_masterAssetsPath);
		return false;
	}

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("MasterAssets");
	if (!pRoot)
	{
		CE_LOG("Project::RegisterScene - '{}' is missing the <MasterAssets> root", m_masterAssetsPath);
		return false;
	}

	for (tinyxml2::XMLElement* pFile = pRoot->FirstChildElement("File"); pFile; pFile = pFile->NextSiblingElement("File"))
	{
		const char* pExisting = pFile->Attribute("path");
		if (pExisting && scenePath == pExisting)
			return true;		// Already registered (re-saving the same scene)
	}

	tinyxml2::XMLElement* pNewFile = doc.NewElement("File");
	pNewFile->SetAttribute("name", sceneName.c_str());
	pNewFile->SetAttribute("path", scenePath.c_str());
	pRoot->InsertEndChild(pNewFile);

	return doc.SaveFile(m_masterAssetsPath.c_str()) == tinyxml2::XML_SUCCESS;
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*---------------------------------------------------------------------------------
| --- ResolveAbsolutePath:
---------------------------------------------------------------------------------*/
std::string Project::ResolveAbsolutePath(const std::string& path)
{
	bool isAbsolute = path.size() >= 2 && path[1] == ':';		// "C:\..." — drive-letter form covers the realistic case here
	return isAbsolute ? path : (s_launchDirectory + "\\" + path);
}