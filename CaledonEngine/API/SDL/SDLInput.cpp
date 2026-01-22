#include "SDLInput.h"
#include "SDL.h"

bool CE::SDLInput::Initialize()
{
	// Initialize the Key Mappings
	m_keyMapping.emplace(SDLK_UP, KeyCode::kUp);
	m_keyMapping.emplace(SDLK_DOWN, KeyCode::kDown);
	m_keyMapping.emplace(SDLK_LEFT, KeyCode::kLeft);
	m_keyMapping.emplace(SDLK_RIGHT, KeyCode::kRight);

	// Initialize the Mouse Mappings
	m_mouseMapping.emplace(SDL_BUTTON_LEFT, MouseCode::kMouseLeft);
	m_mouseMapping.emplace(SDL_BUTTON_RIGHT, MouseCode::kMouseRight);

	return (!m_keyMapping.empty() && !m_mouseMapping.empty()) ? true : false;
}

void CE::SDLInput::Shutdown()
{
    // Clean up if needed.
}

bool CE::SDLInput::ProcessEvents()
{
	m_prevKeyStates = m_keyStates;
	m_prevMouseStates = m_mouseStates;

	m_mouseWheelDelta = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			// Window Event: Closing the Window
		case SDL_QUIT:
			return true;

			// Keyboard Event: Key Pressed
		case SDL_KEYDOWN:
		{
			const auto it = m_keyMapping.find(event.key.keysym.sym);
			if (it != m_keyMapping.end())
			{
				m_keyStates[it->second] = true;
			}
			break;
		}
		// Keyboard Event: Key Released
		case SDL_KEYUP:
		{
			const auto it = m_keyMapping.find(event.key.keysym.sym);
			if (it != m_keyMapping.end())
			{
				m_keyStates[it->second] = false;
			}
			break;
		}
		// Mouse Event: Mouse Button Pressed
		case SDL_MOUSEBUTTONDOWN:
		{
			const auto it = m_mouseMapping.find(event.button.button);
			if (it != m_mouseMapping.end())
			{
				m_mouseStates[it->second] = true;
			}
			break;
		}

		// Mouse Event: Mouse Button Released
		case SDL_MOUSEBUTTONUP:
		{
			const auto it = m_mouseMapping.find(event.button.button);
			if (it != m_mouseMapping.end())
			{
				m_mouseStates[it->second] = false;
			}
			break;
		}

		return false;
		}
	}

	return false;
}