#pragma once
#include "CaledonEngine/Systems/Rendering/ImageLoader.h"

namespace CE
{
    class SDLImageLoader : public ImageLoader
    {
    public:
        SDLImageLoader() = default;
        ~SDLImageLoader() override = default;
        SDLImageLoader(const SDLImageLoader&) = delete;
        SDLImageLoader& operator=(const SDLImageLoader&) = delete;
        SDLImageLoader(SDLImageLoader&&) = delete;
        SDLImageLoader& operator=(SDLImageLoader&&) = delete;

        bool Initialize() override;
        void Shutdown() override;
        std::shared_ptr<Texture> Load(const std::string& filePath,
            Renderer* pRenderer) override;
    };
}