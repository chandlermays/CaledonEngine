/*------------------------------
| File: APIConfig.h
| Author: Chandler Mays
------------------------------*/
#pragma once

#if defined(SDL_DEBUG_CE) || defined(SDL_NDEBUG_CE)

#include "API/SDL/SDLWindow.h"
#include "API/SDL/SDLRenderer.h"
#include "API/SDL/SDLInput.h"
#include "API/SDL/SDLImage.h"

namespace CE
{
	using CEWindow = SDLWindow;
	using CERenderer = SDLRenderer;
	using CEInput = SDLInput;
	using CEImage = SDLImage;
}

#elif defined(SFML_DEBUG_CE) || defined(SFML_NDEBUG_CE)

// SFML includes...

namespace CE
{
	using CEWindow = SFMLWindow;
	using CERenderer = SFMLRenderer;
	using CEInput = SFMLInput;
	using CEImage = SFMLImage;
}

#endif