// ResourceManager.h
#pragma once
#include "CaledonEngine/Systems/Engine/Manager.h"
#include "CaledonEngine/Systems/Rendering/Texture.h"
#include "CaledonEngine/Systems/Rendering/ImageLoader.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace CE
{
    class Renderer;

    class ResourceManager : public Manager
    {
    private:
        std::unique_ptr<ImageLoader> m_pImageLoader;
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureCache;
        Renderer* m_pRenderer;

    public:
        ResourceManager();
        ~ResourceManager();
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        bool Initialize() override;
        void Shutdown() override;

        std::shared_ptr<Texture> LoadTexture(const std::string& filePath);
        void UnloadTexture(const std::string& filePath);
        void UnloadAll();
    };
}