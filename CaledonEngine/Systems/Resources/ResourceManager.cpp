#include "ResourceManager.h"

#include "API/SDL/SDLImage.h"
#include "Systems/Engine/LoggingManager.h"
#include "Utilities/ThirdParty/tinyxml2.h"

#include <fstream>
#include <iostream>

#ifdef NDEBUG
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#endif

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
    CE_LOG("GraphicsManager::~GraphicsManager - Shutting down GraphicsManager.");
    Shutdown();
}

/*------------------------------------------------
| --- Initialize: Setup the Resource Manager --- |
------------------------------------------------*/
bool CE::ResourceManager::Initialize()
{
#ifdef NDEBUG
    // In release mode, the zLib constructor automatically loads and extracts all files
    // from Assets.zip, so no additional initialization is needed here
    std::cout << "ResourceManager initialized in Release mode (using zLib)" << std::endl;
#else
    std::cout << "ResourceManager initialized in Debug mode (loading files directly)" << std::endl;
#endif
    return true;
}

/*-------------------------------------------------
| --- Shutdown: Shutdown the Resource Manager --- |
-------------------------------------------------*/
void CE::ResourceManager::Shutdown()
{
    // Clear all loaded resources
    m_loadedResources.clear();
}

/*------------------------------------------------------------
| --- LoadResource: Load the Data of the Associated File --- |
------------------------------------------------------------*/
bool CE::ResourceManager::LoadResource(const std::string& fileName, std::string& data)
{
    // Check if resource is already loaded
    auto it = m_loadedResources.find(fileName);
    if (it != m_loadedResources.end())
    {
        data = it->second;
        return true;
    }

    std::string resourceData;

#ifdef NDEBUG
    // Release mode: Load from zip file using zLib
    resourceData = m_zLib.GetUncompressedData(fileName);
    if (resourceData.empty())
    {
        std::cout << "Failed to load resource from zip: " << fileName << std::endl;
        return false;
    }
#else
    // Debug mode: Load from file system directly
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    // Read the entire file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    resourceData.resize(fileSize);
    file.read(&resourceData[0], fileSize);
    file.close();
#endif

    // Store in cache and return
    m_loadedResources[fileName] = resourceData;
    data = resourceData;

    std::cout << "Loaded resource: " << fileName << " (" << resourceData.size() << " bytes)" << std::endl;
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
CE::Image* CE::ResourceManager::LoadSurface(const std::string& filePath)
{
#ifdef NDEBUG
    std::string imageData = GetResource(filePath);
    if (imageData.empty())
    {
        std::cout << "Failed to get image data from zip for: " << filePath << std::endl;
        return nullptr;
    }

    SDL_RWops* rw = SDL_RWFromMem(const_cast<char*>(imageData.c_str()), static_cast<int>(imageData.size()));
    if (!rw)
    {
        std::cout << "Failed to create SDL_RWops for: " << filePath << std::endl;
        return nullptr;
    }

    SDL_Surface* surface = IMG_Load_RW(rw, 1); // 1 means SDL will free the RWops
    if (!surface)
    {
        std::cout << "Failed to load surface from zip data for: " << filePath << std::endl;
        return nullptr;
    }

    std::cout << "Loaded surface from zip: " << filePath << " (" << surface->w << "x" << surface->h << ")" << std::endl;
    return new CE::SDLImage(surface);

#else
    CE::SDLImage* pImage = CE::SDLImage::CreateImage(filePath);
    if (!pImage)
    {
        std::cout << "Failed to load image from file: " << filePath << std::endl;
        return nullptr;
    }

    std::cout << "Loaded surface from file: " << filePath << " (" << pImage->GetW() << "x" << pImage->GetH() << ")" << std::endl;
    return pImage;
#endif
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