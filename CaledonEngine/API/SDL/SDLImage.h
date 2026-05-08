#pragma once
#include "CaledonEngine/Systems/Rendering/Image.h"

struct SDL_Surface;

namespace CE
{
    class SDLImage : public Image
    {
    private:
        SDL_Surface* m_pSurface;                                                // The SDL Surface (Image)

    public:
        SDLImage();                                                             // Constructor
        SDLImage(SDL_Surface* pSurface);                                        // Paramaterized Constructor
        ~SDLImage();															// Destructor

        int GetW() const override;                                              // Get the width of the image
        int GetH() const override;											    // Get the height of the image

        static SDLImage* CreateImage(const std::string& filePath);	            // Create an Image from a File

        SDL_Surface* GetNativeImageHandle() const;                              // Returns the API-specified Image
    };
}