#pragma once

#if SDL_DEBUG_CE || SDL_NDEBUG_CE

#include "CaledonEngine/API/SDL/SDLWindow.h"
#include "CaledonEngine/API/SDL/SDLRenderer.h"
#include "CaledonEngine/API/SDL/SDLInput.h"
#include "CaledonEngine/API/SDL/SDLImageLoader.h"

namespace CE
{
	using CEWindow = SDLWindow;
	using CERenderer = SDLRenderer;
	using CEInput = SDLInput;
	using CEImageLoader = SDLImageLoader;
}

#elif SFML_DEBUG_CE || SFML_NDEBUG_CE

// SFML includes go here...

namespace CE
{
	using CEWindow = SFMLWindow;
	using CERenderer = SFMLRenderer;
	using CEInput = SFMLInput;
	using CEImageLoader = SFMLImageLoader;
}

#endif