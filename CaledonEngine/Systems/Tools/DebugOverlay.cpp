/*------------------------------
| File: DebugOverlay.cpp
| Author: Chandler Mays
------------------------------*/
#include "DebugOverlay.h"

#include "Systems/Engine/EngineManager.h"
#include "Systems/Rendering/GraphicsManager.h"
#include "Systems/Rendering/Window.h"
#include "Systems/Rendering/Renderer.h"

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

	// Hook engine diagnostics here.
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