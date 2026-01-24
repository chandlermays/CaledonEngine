#include "GraphicsManager.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Utilities/APIConfig.h"

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
	if (m_pWindowAPI)
	{
		m_pWindowAPI->Initialize();
	}

	m_pRendererAPI = std::make_unique<CERenderer>();
	if (m_pRendererAPI)
	{
		m_pRendererAPI->Initialize(m_pWindowAPI.get());
	}

	return true;
}

/*----------------------------------------------------------
| --- Render: Presents the current frame to the screen --- |
----------------------------------------------------------*/
void CE::GraphicsManager::Render()
{
	m_pRendererAPI->Render();
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

/*---------------------------------------------
| --- GetWindow: Retrieves the Window API --- |
---------------------------------------------*/
CE::Window* CE::GraphicsManager::GetWindow() const
{
	return m_pWindowAPI.get();
}

/*-------------------------------------------------
| --- GetRenderer: Retrieves the Renderer API --- |
-------------------------------------------------*/
CE::Renderer* CE::GraphicsManager::GetRenderer() const
{
	return m_pRendererAPI.get();
}