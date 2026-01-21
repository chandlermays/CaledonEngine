#include "SpriteComponent.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"
#include "SDL.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------
| --- Constructor: Constructs the SpriteComponent with default values --- |
-------------------------------------------------------------------------*/
CE::SpriteComponent::SpriteComponent()
	: Component()
	, m_pRenderer{ nullptr }
	, m_pColor{ nullptr }
	, m_width{ 100 }
	, m_height{ 100 }
{
	// Default to a White color
	m_pColor = new SDL_Color{ 255, 255, 255, 255 };
}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::SpriteComponent::~SpriteComponent()
{
	delete m_pColor;
	m_pColor = nullptr;
}

/*----------------------------------------------------------------------------
| --- Initialize: Prepares the SpriteComponent for use with the Renderer --- |
----------------------------------------------------------------------------*/
bool CE::SpriteComponent::Initialize()
{
	// Cache the renderer from the GraphicsManager
	GraphicsManager* pGraphicsManager = EngineManager::GetInstance().GetGraphicsManager();
	m_pRenderer = pGraphicsManager->GetRenderer();

	return (m_pRenderer != nullptr);
}

/*------------------------------------------------
| --- Render: Draws the sprite to the screen --- |
------------------------------------------------*/
void CE::SpriteComponent::Render()
{
	if (!m_isActive || m_pOwner == nullptr)
		return;

	const Transform& transform = m_pOwner->GetTransform();

	const int x = static_cast<int>(transform.GetPosition().m_x);
	const int y = static_cast<int>(transform.GetPosition().m_y);

	const float sx = transform.GetScale().m_x;
	const float sy = transform.GetScale().m_y;

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;

	const float scaleX = (sx != 0.0f ? sx : 1.0f);
	const float scaleY = (sy != 0.0f ? sy : 1.0f);
	rect.w = static_cast<int>(static_cast<float>(m_width) * scaleX);
	rect.h = static_cast<int>(static_cast<float>(m_height) * scaleY);

	SDL_SetRenderDrawColor(m_pRenderer, m_pColor->r, m_pColor->g, m_pColor->b, m_pColor->a);
	SDL_RenderFillRect(m_pRenderer, &rect);
}

/*---------------------------------------------------
| --- GetColor: Returns the color of the sprite --- |
---------------------------------------------------*/
const SDL_Color* CE::SpriteComponent::GetColor() const
{
	return m_pColor;
}

/*-------------------------------------------------
| --- SetColor: Sets the color of the sprite  --- |
-------------------------------------------------*/
void CE::SpriteComponent::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if (m_pColor != nullptr)
	{
		m_pColor->r = r;
		m_pColor->g = g;
		m_pColor->b = b;
		m_pColor->a = a;
	}
}

/*-------------------------------------------------
| --- GetSize: Returns the size of the sprite --- |
-------------------------------------------------*/
void CE::SpriteComponent::GetSize(int& width, int& height) const
{
	width = m_width;
	height = m_height;
}

/*-----------------------------------------------
| --- SetSize: Sets the size of the sprite  --- |
-----------------------------------------------*/
void CE::SpriteComponent::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}