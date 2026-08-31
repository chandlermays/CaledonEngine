/*------------------------------
| File: GraphicsManager.cpp
| Author: Chandler Mays
------------------------------*/
#include "GraphicsManager.h"

#include "Systems/Engine/LoggingManager.h"
#include "Utilities/APIConfig.h"

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------------
| --- Constructor: Constructs the GraphicsManager with default values --- |
-------------------------------------------------------------------------*/
CE::GraphicsManager::GraphicsManager()
	: m_pWindowAPI{ nullptr }
	, m_pRendererAPI{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::GraphicsManager::~GraphicsManager()
{
	CE_LOG("GraphicsManager::~GraphicsManager - Shutting down GraphicsManager.");
	Shutdown();
}

/*----------------------------------------------------------
| --- Initialize: Prepares the GraphicsManager for use --- |
----------------------------------------------------------*/
bool CE::GraphicsManager::Initialize()
{
	m_pWindowAPI = std::make_unique<CEWindow>();
	if (!m_pWindowAPI || !m_pWindowAPI->Initialize())
	{
		CE_LOG("GraphicsManager::Initialize - Failed to initialize Window.");
		return false;
	}

	m_pRendererAPI = std::make_unique<CERenderer>();
	if (!m_pRendererAPI || !m_pRendererAPI->Initialize(m_pWindowAPI.get()))
	{
		CE_LOG("GraphicsManager::Initialize - Failed to initialize Renderer.");
		return false;
	}

	return true;
}

void CE::GraphicsManager::BeginFrame()
{
	if (m_pRendererAPI)
	{
		m_pRendererAPI->BeginFrame();
	}
}

void CE::GraphicsManager::EndFrame()
{
	if (m_pRendererAPI)
	{
		m_pRendererAPI->EndFrame();
	}
}

/*----------------------------------------------------------------
| --- Shutdown: Cleans up and shuts down the graphics system --- |
----------------------------------------------------------------*/
void CE::GraphicsManager::Shutdown()
{
	if (m_pRendererAPI)
	{
		m_pRendererAPI->Shutdown();
		m_pRendererAPI.reset();
	}
	if (m_pWindowAPI)
	{
		m_pWindowAPI->Shutdown();
		m_pWindowAPI.reset();
	}
}

/*----------------------------------------------------------
| --- GetWindow: Returns a pointer to the Window's API --- |
----------------------------------------------------------*/
CE::Window* CE::GraphicsManager::GetWindow() const
{
	return m_pWindowAPI.get();
}

/*--------------------------------------------------------------
| --- GetRenderer: Returns a pointer to the Renderer's API --- |
--------------------------------------------------------------*/
CE::Renderer* CE::GraphicsManager::GetRenderer() const
{
	return m_pRendererAPI.get();
}