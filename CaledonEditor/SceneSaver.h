/*------------------------------
| File: SceneSaver.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#include <string>

namespace CE
{
	class Scene;
}

class SceneSaver
{
public:
	static bool SaveScene(CE::Scene& scene);																	// 
	static bool SaveSceneAs(CE::Scene& scene, const std::string& newFilePath);									// 

private:
	static bool WriteSceneFile(const std::string& filePath, const std::string& xmlContent);						// 
	static bool EnsureDirectoriesExist(const std::string& filePath);											// 
};