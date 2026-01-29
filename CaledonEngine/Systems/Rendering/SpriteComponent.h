#pragma once
#include "CaledonEngine/Core/Component.h"
#include "CaledonEngine/Systems/Rendering/Color.h"
#include <memory>

namespace CE
{
	class Renderer;
	class Sprite;

	class SpriteComponent : public Component
	{
	private:
		Renderer* m_pRenderer;																		// Pointer to the Renderer
		std::unique_ptr<Sprite> m_pSprite;															// Pointer to the Sprite
		Color m_color;																				// Color of the sprite

	public:
		SpriteComponent();																			// Constructor
		~SpriteComponent() = default;																// Destructor
		SpriteComponent(const SpriteComponent&) = delete;											// Prevent copy-construction
		SpriteComponent& operator=(const SpriteComponent&) = delete;								// Prevent copy-assignment
		SpriteComponent(SpriteComponent&&) = delete;												// Prevent move-construction
		SpriteComponent& operator=(SpriteComponent&&) = delete;										// Prevent move-assignment

		virtual bool Initialize() override;															// Prepares the SpriteComponent for use
		virtual void Render() override;																// Draws the sprite to the screen

		void SetSprite(std::unique_ptr<Sprite> pSprite);											// Sets the sprite to be rendered
		void SetColor(const Color& color);															// Sets the color of the sprite
		void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);			// Sets the color of the sprite

		Sprite* GetSprite() const;																	// Returns a pointer to the sprite
		const Color& GetColor() const;																// Returns the color of the sprite
	};
}