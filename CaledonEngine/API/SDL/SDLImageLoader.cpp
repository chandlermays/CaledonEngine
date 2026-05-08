#include "SDLImageLoader.h"
#include "SDLTexture.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "SDL_image.h"
#include "SDL.h"

bool CE::SDLImageLoader::Initialize()
{
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        CE_LOG("SDLImageLoader::Initialize - SDL_image could not initialize. Error: {}",
            IMG_GetError());
        return false;
    }
    return true;
}

void CE::SDLImageLoader::Shutdown()
{
    IMG_Quit();
}

std::shared_ptr<CE::Texture> CE::SDLImageLoader::Load(const std::string& filePath,
    Renderer* pRenderer)
{
    if (!pRenderer)
    {
        CE_LOG("SDLImageLoader::Load - Renderer is null.");
        return nullptr;
    }

    SDL_Renderer* pSDLRenderer = static_cast<SDL_Renderer*>(pRenderer->GetNativeHandle());
    if (!pSDLRenderer)
    {
        CE_LOG("SDLImageLoader::Load - Native renderer handle is null.");
        return nullptr;
    }

    SDL_Texture* pSDLTexture = IMG_LoadTexture(pSDLRenderer, filePath.c_str());
    if (!pSDLTexture)
    {
        CE_LOG("SDLImageLoader::Load - Failed to load texture: {}. Error: {}",
            filePath, IMG_GetError());
        return nullptr;
    }

    int width = 0;
    int height = 0;
    SDL_QueryTexture(pSDLTexture, nullptr, nullptr, &width, &height);

    auto pTexture = std::make_shared<SDLTexture>();
    if (!pTexture->Load(pSDLTexture, width, height))
    {
        SDL_DestroyTexture(pSDLTexture);
        CE_LOG("SDLImageLoader::Load - Failed to initialize SDLTexture for: {}", filePath);
        return nullptr;
    }

    return pTexture;
}