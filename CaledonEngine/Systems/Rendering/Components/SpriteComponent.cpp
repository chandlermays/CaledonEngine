/*------------------------------
| File: SpriteComponent.cpp
| Author: Chandler Mays
------------------------------*/
#include "SpriteComponent.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Rendering/Shapes/Shape.h"
#include "CaledonEngine/Systems/Rendering/Sprite.h"
#include "CaledonEngine/Systems/Rendering/SpriteSheet.h"
#include "CaledonEngine/Systems/Rendering/Image.h"
#include "CaledonEngine/Systems/Resources/ResourceManager.h"
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
{ }

/*-------------------------------------------------------------------------------
| --- Constructor: Constructs the SpriteComponent with parameterized values --- |
-------------------------------------------------------------------------------*/
CE::SpriteComponent::SpriteComponent(const char* pSheet, int width, int height, float scale)
	: Component()
	, m_pRenderer{ nullptr }
	, m_pSprite{ nullptr }
	, m_color{ 255, 255, 255, 255 }
{
	LoadSpriteSheet(pSheet, width, height, scale);
}

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

	const int x = static_cast<int>(transform.GetPosition().x);
	const int y = static_cast<int>(transform.GetPosition().y);

	const float sx = transform.GetScale().x;
	const float sy = transform.GetScale().y;

	const float scaleX = (sx != 0.0f ? sx : 1.0f);
	const float scaleY = (sy != 0.0f ? sy : 1.0f);

	const Vector2f& spriteSize = m_pSprite->GetSize();
	const Vector2f& pivot = m_pSprite->GetPivot();

	int pixelWidth = static_cast<int>(m_pSprite->GetPixelWidth() * scaleX);
	int pixelHeight = static_cast<int>(m_pSprite->GetPixelHeight() * scaleY);

	int offsetX = static_cast<int>(-pivot.x * pixelWidth);
	int offsetY = static_cast<int>(-pivot.y * pixelHeight);

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
				m_pRenderer->SetTextureColorMod(pTexture, m_color.r, m_color.g, m_color.b);
				m_pRenderer->SetTextureAlphaMod(pTexture, m_color.a);
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
				renderColor.r = static_cast<uint8_t>((renderColor.r * m_color.r) / 255);
				renderColor.g = static_cast<uint8_t>((renderColor.g * m_color.g) / 255);
				renderColor.b = static_cast<uint8_t>((renderColor.b * m_color.b) / 255);
				renderColor.a = static_cast<uint8_t>((renderColor.a * m_color.a) / 255);
			}
			pShape->Render(m_pRenderer, destRect, renderColor);
		}
	}
}

/*----------------------------------------------------------------------------------------------
| --- LoadSpritesheet: Loads a sprite from a spritesheet and sets it as the current sprite --- |
----------------------------------------------------------------------------------------------*/
void CE::SpriteComponent::LoadSpriteSheet(const char* pSheet, int frameWidth, int frameHeight, float scale)
{
	if (!pSheet)
		return;

	GraphicsManager* pGraphicsManager = EngineManager::GetInstance().GetGraphicsManager();
	Renderer* pRenderer = pGraphicsManager ? pGraphicsManager->GetRenderer() : nullptr;
	ResourceManager* pResourceManager = EngineManager::GetInstance().GetResourceManager();

	if (!pRenderer || !pResourceManager)
		return;

	std::unique_ptr<Image> pImage(pResourceManager->LoadSurface(pSheet));
	if (!pImage)
		return;

	std::shared_ptr<Texture> pTexture = pRenderer->CreateTexture(pImage.get());
	if (!pTexture)
		return;

	m_pSpriteSheet = std::make_unique<SpriteSheet>(pTexture, frameWidth, frameHeight);
	m_pSprite = m_pSpriteSheet->CreateSprite(0);

	(void)scale;	// TODO: decide how 'scale' maps to Transform scale / pixelsPerUnit once that's settled
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
void CE::SpriteComponent::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

/*---------------------------------------------------------------
| --- GetSpriteSheet: Returns a pointer to the sprite sheet --- |
---------------------------------------------------------------*/
CE::SpriteSheet* CE::SpriteComponent::GetSpriteSheet() const
{
	return m_pSpriteSheet.get();
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