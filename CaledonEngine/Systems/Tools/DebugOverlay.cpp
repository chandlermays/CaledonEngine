/*------------------------------
| File: DebugOverlay.cpp
| Author: Chandler Mays
------------------------------*/
#include "DebugOverlay.h"

#include "Systems/Engine/EngineManager.h"
#include "Systems/Rendering/GraphicsManager.h"
#include "Systems/Rendering/Window.h"
#include "Systems/Rendering/Renderer.h"
#include "Systems/Physics/CollisionManager.h"

#include <SDL3/SDL.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*----------------------------------------------------------------------
| --- Constructor: Constructs the DebugOverlay with default values --- |
----------------------------------------------------------------------*/
CE::DebugOverlay::DebugOverlay()
	: m_isVisible{ false }
{ }

/*----------------------------------------------------------
| --- Initialize: Initializes the DebugOverlay for use --- |
----------------------------------------------------------*/
bool CE::DebugOverlay::Initialize()
{
	return true;
}

/*-------------------------------------------------------
| --- Draw: Draws the DebugOverlay if it is visible --- |
-------------------------------------------------------*/
void CE::DebugOverlay::Draw()
{
	if (!m_isVisible)
		return;

	auto* pGraphics = EngineManager::GetInstance().GetGraphicsManager();
	auto* pCollision = EngineManager::GetInstance().GetCollisionManager();

	if (!pGraphics || !pCollision)
		return;

	auto* pRenderer = pGraphics->GetRenderer();
	if (!pRenderer)
		return;

	pCollision->RefreshAllBounds();

	// Draw visual outlines for active colliders
	const Color outlineColor{ 0, 255, 0, 255 };			// Green color for debug outlines

	for (const Collider2D* pCollider : pCollision->GetActiveColliders())
	{
		if (!pCollider || !pCollider->IsActive() || !pCollider->GetOwner())
			continue;

		const AABB2D& bounds = pCollider->GetBounds();
		Vector2f size = bounds.GetSize();

		// Convert AABB2D to RectFloat (min position x/y and size width/height)
		RectFloat rect(bounds.min.x, bounds.min.y, size.x, size.y);

		// Draw outline rectangle
		pRenderer->DrawRect(rect, outlineColor, false);
	}
}

/*------------------------------------------
| --- Shutdown: Cleans up the viewport --- |
------------------------------------------*/
void CE::DebugOverlay::Shutdown()
{
	//...
}

/*-------------------------------------------------------------
| --- SetVisible: Sets the visibility of the DebugOverlay --- |
-------------------------------------------------------------*/
void CE::DebugOverlay::SetVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

/*----------------------------------------------------------------------
| --- ToggleVisibility: Toggles the visibility of the DebugOverlay --- |
----------------------------------------------------------------------*/
void CE::DebugOverlay::ToggleVisibility()
{
	m_isVisible = !m_isVisible;
}

/*--------------------------------------------------------------------------
| --- IsVisible: Returns whether the DebugOverlay is currently visible --- |
--------------------------------------------------------------------------*/
bool CE::DebugOverlay::IsVisible() const
{
	return m_isVisible;
}