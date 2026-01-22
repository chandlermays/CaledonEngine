#pragma once

#if SDL_DEBUG_CE || SDL_NDEBUG_CE

#include "CaledonEngine/API/SDL/SDLInput.h"

namespace CE
{
	using CEInput = SDLInput;
}

#elif SFML_DEBUG_CE || SFML_NDEBUG_CE

//includes

namespace CE
{
	//using declarations
}

#endif