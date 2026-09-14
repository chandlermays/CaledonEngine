/*------------------------------
| File: Project.cpp
| Author: Chandler Mays
------------------------------*/
#include "Project.h"

#include "CaledonEngine/Utilities/CaledonParser.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"

#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"
#include <Windows.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------
| --- Load: Loads the project from the specified file path --- |
--------------------------------------------------------------*/
bool Project::Load(const std::string& projectFilePath)
{
	CE::CaledonParser parser;
	if (!parser.LoadFile(projectFilePath))
	{
		CE_LOG("Project::Load - Could not open '{}'", projectFilePath);
		return false;
	}

	tinyxml2::XMLElement* pRoot = parser.GetRootElement("CaledonProject");
	if (!pRoot)
	{
		CE_LOG("Project::Load - '{}' is missing the <CaledonProject> root element", projectFilePath);
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

	// NOTE: The project file's own directory becomes the working directory for everything
	// downstream — module loading, ResourceManager. Neither needs to change at all;
	// they just keep using plain relative paths, exactly as Game.exe already does.
	// This is what makes a project folder self-contained and genuinely portable.
	size_t lastSlash = projectFilePath.find_last_of("\\/");
	m_rootDirectory = (lastSlash != std::string::npos) ? projectFilePath.substr(0, lastSlash) : ".";

	if (!SetCurrentDirectoryA(m_rootDirectory.c_str()))
	{
		CE_LOG("Project::Load - Could not set working directory to '{}'", m_rootDirectory);
		return false;
	}

	return !m_modulePath.empty();
}

/*--------------------------------------------------------------------------------------------------
| --- CreateNew: Creates a new project in the specified root directory with the specified name --- |
--------------------------------------------------------------------------------------------------*/
bool Project::CreateNew(const std::string& rootDirectory, const std::string& projectName)
{
	CreateDirectoryA(rootDirectory.c_str(), nullptr);

	std::string assetsDirectory = rootDirectory + "\\Assets";
	CreateDirectoryA(assetsDirectory.c_str(), nullptr);

	std::ofstream masterAssetsFile(assetsDirectory + "\\MasterAssets.xml");
	if (masterAssetsFile.is_open())
	{
		masterAssetsFile << "<MasterAssets>\n</MasterAssets>\n";
		masterAssetsFile.close();
	}

	std::string projectFilePath = rootDirectory + "\\" + projectName + ".ceproj";
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