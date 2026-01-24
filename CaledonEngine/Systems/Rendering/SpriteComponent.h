#pragma once
#include "CaledonEngine/Core/Component.h"

class SDL_Color;

namespace CE
{
	class Renderer;

	class SpriteComponent : public Component
	{
	private:
		Renderer* m_pRenderer;																		// Pointer to the Renderer
		SDL_Color* m_pColor;																		// Pointer to the color of the sprite
		int m_width;																				// Width of the sprite
		int m_height;																				// Height of the sprite

	public:
		SpriteComponent();																			// Constructor
		~SpriteComponent();																			// Destructor
		SpriteComponent(const SpriteComponent&) = delete;											// Prevent copy-construction
		SpriteComponent& operator=(const SpriteComponent&) = delete;								// Prevent copy-assignment
		SpriteComponent(SpriteComponent&&) = delete;												// Prevent move-construction
		SpriteComponent& operator=(SpriteComponent&&) = delete;										// Prevent move-assignment

		virtual bool Initialize() override;															// Prepares the SpriteComponent for use
		virtual void Render() override;																// Draws the sprite to the screen

		const SDL_Color* GetColor() const;															// Returns the color of the sprite
		void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);			// Sets the color of the sprite

		void GetSize(int& width, int& height) const;												// Returns the size of the sprite
		void SetSize(int width, int height);														// Sets the size of the sprite
	};
}