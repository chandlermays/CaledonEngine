/*------------------------------
| File: SpriteComponent.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include "CaledonEngine/Core/Component.h"
#include "CaledonEngine/Systems/Rendering/Color.h"
#include <memory>

namespace CE
{
	class SpriteSheet;
	class Renderer;
	class Sprite;

	class SpriteComponent : public Component
	{
	private:
		std::unique_ptr<SpriteSheet> m_pSpriteSheet;												// Pointer to the SpriteSheet
		Renderer* m_pRenderer;																		// Pointer to the Renderer
		std::unique_ptr<Sprite> m_pSprite;															// Pointer to the Sprite
		Color m_color;																				// Color of the sprite

	public:
		SpriteComponent();																			// Constructor
		SpriteComponent(const char* pSheet, int width, int height, float scale);					// Parameterized Constructor
		~SpriteComponent();																			// Destructor
		SpriteComponent(const SpriteComponent&) = delete;											// Prevent copy-construction
		SpriteComponent& operator=(const SpriteComponent&) = delete;								// Prevent copy-assignment
		SpriteComponent(SpriteComponent&&) = delete;												// Prevent move-construction
		SpriteComponent& operator=(SpriteComponent&&) = delete;										// Prevent move-assignment

		virtual bool Initialize() override;															// Prepares the SpriteComponent for use
		virtual void Render() override;																// Draws the sprite to the screen

		void LoadSpriteSheet(const char* pSheet, int width, int height, float scale);				// Loads a sprite from a spritesheet
		void SetSprite(std::unique_ptr<Sprite> pSprite);											// Sets the sprite to be rendered
		void SetColor(const Color& color);															// Sets the color of the sprite
		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);									// Sets the color of the sprite

		SpriteSheet* GetSpriteSheet() const;														// Returns a pointer to the sprite sheet
		Sprite* GetSprite() const;																	// Returns a pointer to the sprite
		const Color& GetColor() const;																// Returns the color of the sprite
	};
}