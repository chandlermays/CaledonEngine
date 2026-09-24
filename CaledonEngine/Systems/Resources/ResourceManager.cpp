/*------------------------------
| File: ResourceManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "ResourceManager.h"

#include "API/SDL/SDLImage.h"
#include "Systems/Engine/LoggingManager.h"
#include "Utilities/ThirdParty/tinyxml2.h"

#include <fstream>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------
| --- Constructor: Constructs the ResourceManager with default values --- |
-------------------------------------------------------------------------*/
CE::ResourceManager::ResourceManager()
{
    // Constructor body - initialization handled in Initialize()
}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::ResourceManager::~ResourceManager()
{
    Shutdown();
}

/*------------------------------------------------
| --- Initialize: Setup the Resource Manager --- |
------------------------------------------------*/
bool CE::ResourceManager::Initialize()
{
    std::cout << "ResourceManager initialized (loading files directly)" << std::endl;
    return true;
}

/*-------------------------------------------------
| --- Shutdown: Shutdown the Resource Manager --- |
-------------------------------------------------*/
void CE::ResourceManager::Shutdown()
{
    ClearCache();
}

/*--------------------------------------------------------------
| --- ClearCache: Clear the collection of loaded resources --- |
--------------------------------------------------------------*/
void CE::ResourceManager::ClearCache()
{
    m_loadedResources.clear();
}

/*------------------------------------------------------------
| --- LoadResource: Load the Data of the Associated File --- |
------------------------------------------------------------*/
bool CE::ResourceManager::LoadResource(const std::string& fileName, std::string& data)
{
    auto it = m_loadedResources.find(fileName);
    if (it != m_loadedResources.end())
    {
        data = it->second;
        return true;
    }

    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    std::streamsize fileSize = file.tellg();
    if (fileSize < 0)
    {
        std::cout << "Failed to determine size of file: " << fileName << std::endl;
        return false;
    }

    file.seekg(0, std::ios::beg);

    std::string resourceData(static_cast<size_t>(fileSize), '\0');
    if (fileSize > 0 && !file.read(resourceData.data(), fileSize))
    {
        std::cout << "Failed to read file: " << fileName << std::endl;
        return false;
    }

    data = resourceData;
    m_loadedResources[fileName] = std::move(resourceData);

    std::cout << "Loaded resource: " << fileName << " (" << data.size() << " bytes)" << std::endl;
    return true;
}

/*--------------------------------------------------------------
| --- GetResource: Returns the Data of the Associated File --- |
--------------------------------------------------------------*/
std::string CE::ResourceManager::GetResource(const std::string& fileName)
{
    // Check if already loaded
    auto it = m_loadedResources.find(fileName);
    if (it != m_loadedResources.end())
    {
        return it->second;
    }

    // If not loaded, try to load it
    std::string data;
    if (LoadResource(fileName, data))
    {
        return data;
    }

    return std::string(); // Return empty string if failed to load
}

/*-------------------------------------------------
| --- LoadSurface: Load a Surface from a File --- |
-------------------------------------------------*/
std::unique_ptr<CE::Image> CE::ResourceManager::LoadSurface(const std::string& filePath)
{
    std::unique_ptr<CE::SDLImage> pImage = CE::SDLImage::CreateImage(filePath);
    if (!pImage)
    {
        std::cout << "Failed to load image from file: " << filePath << std::endl;
        return nullptr;
    }

    std::cout << "Loaded surface from file: " << filePath << " (" << pImage->GetW() << "x" << pImage->GetH() << ")" << std::endl;
    return pImage;
}

/*---------------------------------------------------------------------------------------
| --- LoadMasterXML: Loads a master XML file, returns its {name, path} file entries --- |
---------------------------------------------------------------------------------------*/
std::unordered_map<std::string, std::string> CE::ResourceManager::LoadMasterXML(const std::string& filePath)
{
    std::string xmlData = GetResource(filePath);
    if (xmlData.empty())
    {
        std::cout << "Failed to load master XML: " << filePath << std::endl;
        return {};
    }

    if (!m_parser.Parse(xmlData))
    {
        std::cout << "Failed to parse master XML: " << filePath << std::endl;
        return {};
    }

    // Expects: <MasterAssets><File name="..." path="..."/>...</MasterAssets>
    auto entries = m_parser.ExtractStructure("MasterAssets", "File", "name", "path");

    std::cout << "Loaded master XML: " << filePath << " (" << entries.size() << " entries)" << std::endl;
    return entries;
}