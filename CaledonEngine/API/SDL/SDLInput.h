#pragma once
#include "CaledonEngine/Systems/Input/Input.h"

namespace CE
{
	typedef int SDL_KeyCode;
	typedef int SDL_MouseCode;

	class SDLInput : public Input
	{
	private:
		std::unordered_map<SDL_KeyCode, KeyCode> m_keyMapping;
		std::unordered_map<SDL_MouseCode, MouseCode> m_mouseMapping;

	public:
		SDLInput() = default;
		~SDLInput() override = default;

		bool Initialize() override;
		void Shutdown() override;
		bool ProcessEvents() override;
	};
}