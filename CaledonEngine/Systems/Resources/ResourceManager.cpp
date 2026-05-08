#include "ResourceManager.h"
#include "CaledonEngine/API/SDL/SDLImage.h"
#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"

#include <fstream>
#include <iostream>
#include <cassert>

#ifdef NDEBUG
#include "SDL.h"
#include "SDL_image.h"
#endif

///////////////////////////////////////
/*-------------------------------------
| --- Public Function Definitions --- |
-------------------------------------*/
///////////////////////////////////////

/*---------------------
| --- Constructor --- |
---------------------*/

CE::ResourceManager::ResourceManager()
{
    // Constructor body - initialization handled in Initialize()
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
    std::cout << "ResourceManager shutdown complete" << std::endl;
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
    // Release mode: Load from zip file data
    std::string imageData = GetResource(filePath);
    if (imageData.empty())
    {
        std::cout << "Failed to get image data from zip for: " << filePath << std::endl;
        return nullptr;
    }

    // Create SDL_RWops from memory buffer
    SDL_RWops* rw = SDL_RWFromMem(const_cast<char*>(imageData.c_str()), static_cast<int>(imageData.size()));
    assert(rw);

    // Load the surface from memory
    SDL_Surface* surface = IMG_Load_RW(rw, 1); // 1 means SDL will free the RWops
    assert(surface);

    std::cout << "Loaded surface from zip: " << filePath << " (" << surface->w << "x" << surface->h << ")" << std::endl;
    return new CE::SDLImage(surface);

#else
    // Debug mode: Load directly from file system
    CE::SDLImage* image = CE::SDLImage::CreateImage(filePath);
    assert(image);

    std::cout << "Loaded surface from file: " << filePath << " (" << image->GetW() << "x" << image->GetH() << ")" << std::endl;
    return image;
#endif
}

/*-----------------------------------------------
| --- LoadMasterXML: Load a Master XML File --- |
-----------------------------------------------*/

bool CE::ResourceManager::LoadMasterXML(const std::string& filePath)
{
    std::string xmlData = GetResource(filePath);
    if (xmlData.empty())
    {
        std::cout << "Failed to load master XML: " << filePath << std::endl;
        return false;
    }

    // Parse the XML data using your CaledonParser
    if (!m_parser.Parse(xmlData))
    {
        std::cout << "Failed to parse master XML: " << filePath << std::endl;
        return false;
    }

    std::cout << "Successfully loaded and parsed master XML: " << filePath << std::endl;
    return true;
}