#include "SpriteComponent.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Engine/EngineManager.h"
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
	, m_color{ 255, 255, 255, 255 }
	, m_width{ 100 }
	, m_height{ 100 }
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

	if (!m_isActive || !m_pOwner || !m_pRenderer)
		return;

	const Transform& transform = m_pOwner->GetTransform();

	const int x = static_cast<int>(transform.GetPosition().m_x);
	const int y = static_cast<int>(transform.GetPosition().m_y);

	const float sx = transform.GetScale().m_x;
	const float sy = transform.GetScale().m_y;

	const float scaleX = (sx != 0.0f ? sx : 1.0f);
	const float scaleY = (sy != 0.0f ? sy : 1.0f);

	Rect rect
	{
		x,
		y,
		static_cast<int>(m_width * scaleX),
		static_cast<int>(m_height * scaleY)
	};

	m_pRenderer->DrawRect(rect, m_color);
}

/*---------------------------------------------------
| --- GetColor: Returns the color of the sprite --- |
---------------------------------------------------*/
const CE::Color& CE::SpriteComponent::GetColor() const
{
	return m_color;
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
