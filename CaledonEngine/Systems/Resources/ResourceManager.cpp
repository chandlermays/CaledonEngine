#include "ResourceManager.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Utilities/APIConfig.h"

CE::ResourceManager::ResourceManager()
    : m_pImageLoader{ nullptr }
    , m_pRenderer{ nullptr }
{}

CE::ResourceManager::~ResourceManager()
{
    CE_LOG("ResourceManager::~ResourceManager - Shutting down ResourceManager.");
    Shutdown();
}

bool CE::ResourceManager::Initialize()
{
    m_pRenderer = EngineManager::GetInstance().GetGraphicsManager()->GetRenderer();
    if (!m_pRenderer)
    {
        CE_LOG("ResourceManager::Initialize - Renderer is null.");
        return false;
    }

    m_pImageLoader = std::make_unique<CEImageLoader>();
    if (!m_pImageLoader->Initialize())
    {
        CE_LOG("ResourceManager::Initialize - Failed to initialize ImageLoader.");
        return false;
    }

    return true;
}

void CE::ResourceManager::Shutdown()
{
    UnloadAll();

    if (m_pImageLoader)
    {
        m_pImageLoader->Shutdown();
        m_pImageLoader.reset();
    }

    m_pRenderer = nullptr;
}

std::shared_ptr<CE::Texture> CE::ResourceManager::LoadTexture(const std::string& filePath)
{
    // Return cached texture if already loaded
    auto it = m_textureCache.find(filePath);
    if (it != m_textureCache.end())
    {
        return it->second;
    }

    // Otherwise load and cache it
    auto pTexture = m_pImageLoader->Load(filePath, m_pRenderer);
    if (!pTexture)
    {
        CE_LOG("ResourceManager::LoadTexture - Failed to load: {}", filePath);
        return nullptr;
    }

    m_textureCache.emplace(filePath, pTexture);
    return pTexture;
}

void CE::ResourceManager::UnloadTexture(const std::string& filePath)
{
    m_textureCache.erase(filePath);
}

void CE::ResourceManager::UnloadAll()
{
    m_textureCache.clear();
}