#pragma once
#include <memory>
#include <string>

namespace CE
{
    class Texture;
    class Renderer;

    class ImageLoader
    {
    public:
        virtual ~ImageLoader() = default;
        ImageLoader(const ImageLoader&) = delete;
        ImageLoader& operator=(const ImageLoader&) = delete;
        ImageLoader(ImageLoader&&) = delete;
        ImageLoader& operator=(ImageLoader&&) = delete;

        virtual bool Initialize() = 0;
        virtual void Shutdown() = 0;

        virtual std::shared_ptr<Texture> Load(const std::string& filePath, Renderer* pRenderer) = 0;

    protected:
        ImageLoader() = default;
    };
}