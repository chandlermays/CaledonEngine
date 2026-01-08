#pragma once
#include "../Core/Component.h"

class SDL_Renderer;
class SDL_Color;

namespace CE
{
	class SpriteComponent : public Component
	{
	private:
		SDL_Renderer* m_pRenderer;
		SDL_Color* m_pColor;

	public:
		SpriteComponent();
		~SpriteComponent();
		SpriteComponent(const SpriteComponent&) = delete;
		SpriteComponent& operator=(const SpriteComponent&) = delete;
		SpriteComponent(SpriteComponent&&) = delete;
		SpriteComponent& operator=(SpriteComponent&&) = delete;

		virtual bool Initialize() override;
		virtual void Render() override;

		const SDL_Color* GetColor() const;
		void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	};
}