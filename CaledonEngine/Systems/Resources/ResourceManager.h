/*------------------------------
| File: ResourceManager.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "Systems/Engine/Manager.h"
#include "Systems/Rendering/Image.h"
#include "Systems/Rendering/Texture.h"
#include "Utilities/CaledonParser.h"
#include "Utilities/ThirdParty/CaledonZLib.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace CE
{
	class ResourceManager : public Manager
	{
	private:
#ifdef NDEBUG
		CaledonZLib m_zLib;																				// zLib Compression
#endif
		std::unordered_map<std::string, std::string> m_loadedResources;									// A Collection of Loaded Resources
		CaledonParser m_parser;																			// Parser for the XML Files

	public:
		ResourceManager();																				// Constructor
		~ResourceManager();																				// Destructor

		virtual bool Initialize() override;																// Initialize the Resource Manager
		virtual void Shutdown() override;																// Shutdown the Resource Manager

		bool LoadResource(const std::string& fileName, std::string& data);								// Load the Data of the Associated File
		std::string GetResource(const std::string& fileName);											// Returns the Data of the Associated File

		CE::Image* LoadSurface(const std::string& filePath);											// Load the Image File

		std::unordered_map<std::string, std::string> LoadMasterXML(const std::string& filePath);		// Loads a master XML file, returns its {name, path} file entries
	};
}