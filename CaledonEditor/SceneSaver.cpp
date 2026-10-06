/*------------------------------
| File: SceneSaver.cpp
| Author: Chandler Mays
------------------------------*/
#include "SceneSaver.h"

#include "CaledonEngine/Core/Scene.h"
#include "CaledonEngine/GameObjectSerializer.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"

#include <fstream>
#include <filesystem>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------------
| --- SaveScene:
------------------------------------------------------------------------------*/
bool SceneSaver::SaveScene(CE::Scene& scene)
{
	if (scene.GetFilePath().empty())
	{
		CE_LOG("SceneSaver::SaveScene - Scene has no file path set");
		return false;
	}

	return SaveSceneAs(scene, scene.GetFilePath());
}

/*------------------------------------------------------------------------------
| --- SaveSceneAs:
------------------------------------------------------------------------------*/
bool SceneSaver::SaveSceneAs(CE::Scene& scene, const std::string& newFilePath)
{
	if (newFilePath.empty())
	{
		CE_LOG("SceneSaver::SaveSceneAs - File path is empty");
		return false;
	}

	// Ensure directories exist
	if (!EnsureDirectoriesExist(newFilePath))
	{
		CE_LOG("SceneSaver::SaveSceneAs - Failed to create directories for '{}'", newFilePath);
		return false;
	}

	// Serialize the scene
	std::string xmlContent = CE::GameObjectSerializer::SerializeScene(scene);
	if (xmlContent.empty())
	{
		CE_LOG("SceneSaver::SaveSceneAs - Serialization failed for scene '{}'", scene.GetName());
		return false;
	}

	// Write to disk
	if (!WriteSceneFile(newFilePath, xmlContent))
	{
		CE_LOG("SceneSaver::SaveSceneAs - Failed to write scene to '{}'", newFilePath);
		return false;
	}

	// Update scene state
	scene.SetFilePath(newFilePath);
	scene.MarkClean();

	CE_LOG("SceneSaver::SaveSceneAs - Saved scene '{}' to '{}'", scene.GetName(), newFilePath);
	return true;
}



/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------
| --- WriteSceneFile:
------------------------------------------------------------------------------*/
bool SceneSaver::WriteSceneFile(const std::string& filePath, const std::string& xmlContent)
{
	try
	{
		std::ofstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			CE_LOG("SceneSaver::WriteSceneFile - Could not open file for writing: '{}'", filePath);
			return false;
		}

		file.write(xmlContent.c_str(), xmlContent.size());
		file.close();

		return true;
	}
	catch (const std::exception& e)
	{
		CE_LOG("SceneSaver::WriteSceneFile - Exception: {}", e.what());
		return false;
	}
}

/*------------------------------------------------------------------------------
| --- EnsureDirectoriesExist: 
------------------------------------------------------------------------------*/
bool SceneSaver::EnsureDirectoriesExist(const std::string& filePath)
{
	try
	{
		std::filesystem::path path(filePath);
		std::filesystem::path dirPath = path.parent_path();

		if (!dirPath.empty() && !std::filesystem::exists(dirPath))
		{
			std::filesystem::create_directories(dirPath);
		}

		return true;
	}
	catch (const std::exception& e)
	{
		CE_LOG("SceneSaver::EnsureDirectoriesExist - Exception: {}", e.what());
		return false;
	}
}