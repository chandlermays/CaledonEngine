#include "SpriteComponent.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/Sprite.h"
#include "CaledonEngine/Systems/Rendering/Shape.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------
| --- Constructor: Constructs the SpriteComponent with default values --- |
-------------------------------------------------------------------------*/
CE::SpriteComponent::SpriteComponent()
	: Component()
	, m_pRenderer{ nullptr }
	, m_pSprite{ nullptr }
	, m_color{ 255, 255, 255, 255 }
{}

/*----------------------------------------------------------
| --- Initialize: Prepares the SpriteComponent for use --- |
----------------------------------------------------------*/
bool CE::SpriteComponent::Initialize()
{
	// Cache the renderer from the GraphicsManager
	GraphicsManager* pGraphicsManager = EngineManager::GetInstance().GetGraphicsManager();
	if (pGraphicsManager)
	{
		m_pRenderer = pGraphicsManager->GetRenderer();
	}

	return (m_pRenderer != nullptr);
}

/*------------------------------------------------
| --- Render: Draws the sprite to the screen --- |
------------------------------------------------*/
void CE::SpriteComponent::Render()
{
	// Add error logging if this Render method breaks early (with an explanation).

	if (!m_isActive || !m_pOwner || !m_pRenderer || !m_pSprite)
		return;

	const Transform& transform = m_pOwner->GetTransform();

	const int x = static_cast<int>(transform.GetPosition().m_x);
	const int y = static_cast<int>(transform.GetPosition().m_y);

	const float sx = transform.GetScale().m_x;
	const float sy = transform.GetScale().m_y;

	const float scaleX = (sx != 0.0f ? sx : 1.0f);
	const float scaleY = (sy != 0.0f ? sy : 1.0f);

	const VectorFloat& spriteSize = m_pSprite->GetSize();
	const VectorFloat& pivot = m_pSprite->GetPivot();

	int pixelWidth = static_cast<int>(m_pSprite->GetPixelWidth() * scaleX);
	int pixelHeight = static_cast<int>(m_pSprite->GetPixelHeight() * scaleY);

	int offsetX = static_cast<int>(-pivot.m_x * pixelWidth);
	int offsetY = static_cast<int>(-pivot.m_y * pixelHeight);

	Rect destRect
	{
		x + offsetX,
		y + offsetY,
		pixelWidth,
		pixelHeight
	};

	if (m_pSprite->GetType() == SpriteType::kTexture)
	{
		Texture* pTexture = m_pSprite->GetTexture();
		if (pTexture)
		{
			Rect srcRect = m_pSprite->GetTextureRect();

			if (m_color != Color::White())
			{
				m_pRenderer->SetTextureColorMod(pTexture, m_color.m_r, m_color.m_g, m_color.m_b);
				m_pRenderer->SetTextureAlphaMod(pTexture, m_color.m_a);
			}

			m_pRenderer->RenderCopy(pTexture, &srcRect, &destRect);

			if (m_color != Color::White())
			{
				m_pRenderer->SetTextureColorMod(pTexture, 255, 255, 255);
				m_pRenderer->SetTextureAlphaMod(pTexture, 255);
			}
		}
	}
	else if (m_pSprite->GetType() == SpriteType::kPrimitive)
	{
		Shape* pShape = m_pSprite->GetShape();
		if (pShape)
		{
			Color renderColor = pShape->GetColor();
			if (m_color != Color::White())
			{
				renderColor.m_r = static_cast<unsigned char>((renderColor.m_r * m_color.m_r) / 255);
				renderColor.m_g = static_cast<unsigned char>((renderColor.m_g * m_color.m_g) / 255);
				renderColor.m_b = static_cast<unsigned char>((renderColor.m_b * m_color.m_b) / 255);
				renderColor.m_a = static_cast<unsigned char>((renderColor.m_a * m_color.m_a) / 255);
			}

			Color originalColor = pShape->GetColor();
			const_cast<Shape*>(pShape)->SetColor(renderColor);

			pShape->Render(m_pRenderer, destRect);

			const_cast<Shape*>(pShape)->SetColor(originalColor);
		}
	}
}

/*---------------------------------------------------
| --- SetSprite: Sets the sprite to be rendered --- |
---------------------------------------------------*/
void CE::SpriteComponent::SetSprite(std::unique_ptr<Sprite> pSprite)
{
	m_pSprite = std::move(pSprite);
}

/*-------------------------------------------------
| --- SetColor: Sets the color of the sprite  --- |
-------------------------------------------------*/
void CE::SpriteComponent::SetColor(const Color& color)
{
	m_color = color;
}

/*-------------------------------------------------
| --- SetColor: Sets the color of the sprite  --- |
-------------------------------------------------*/
void CE::SpriteComponent::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_color.m_r = r;
	m_color.m_g = g;
	m_color.m_b = b;
	m_color.m_a = a;
}

/*----------------------------------------------------
| --- GetSprite: Returns a pointer to the sprite --- |
----------------------------------------------------*/
CE::Sprite* CE::SpriteComponent::GetSprite() const
{
	return m_pSprite.get();
}

/*---------------------------------------------------
| --- GetColor: Returns the color of the sprite --- |
---------------------------------------------------*/
const CE::Color& CE::SpriteComponent::GetColor() const
{
	return m_color;
}