#include "GraphicsManager.h"
#include "CaledonEngine/Engine/LoggingManager.h"
#include "SDL.h"

static constexpr int s_kWindowWidth = 1080;
static constexpr int s_kWindowHeight = 720;

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------
| --- Constructor: Sets initial values for members --- |
------------------------------------------------------*/
CE::GraphicsManager::GraphicsManager()
	: m_pWindow{ nullptr }
	, m_pRenderer{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::GraphicsManager::~GraphicsManager()
{
	Shutdown();
}

/*------------------------------------------------------
| --- Initialize: Initializes the graphics manager --- |
------------------------------------------------------*/
bool CE::GraphicsManager::Initialize()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		CE_LOG("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
		return false;
	}

	// Create window
	m_pWindow = SDL_CreateWindow("Caledon Engine",	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, s_kWindowWidth, s_kWindowHeight, 0);

	if (m_pWindow == nullptr)
	{
		CE_LOG("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
		SDL_Quit();
		return false;
	}

	// Create renderer
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

	if (m_pRenderer == nullptr)
	{
		CE_LOG("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
		SDL_Quit();
		return false;
	}

	return true;
}

/*----------------------------------------------
| --- Update: Updates the graphics manager --- |
----------------------------------------------*/
void CE::GraphicsManager::Update(float deltaTime)
{
	(void)deltaTime;
	//...
}

/*-------------------------------------------
| --- Render: Renders the current frame --- |
-------------------------------------------*/
void CE::GraphicsManager::Render()
{
	// Clear the screen with a blue color
	SDL_SetRenderDrawColor(m_pRenderer, 0, 156, 255, 255);

	// Present the rendered frame
	SDL_RenderPresent(m_pRenderer);

	// Clear the renderer for the next frame
	SDL_RenderClear(m_pRenderer);
}

/*---------------------------------------------------
| --- Shutdown: Shuts down the graphics manager --- |
---------------------------------------------------*/
void CE::GraphicsManager::Shutdown()
{
	// Destroy renderer
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}

	// Destroy window
	if (m_pWindow != nullptr)
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}

	// Quit SDL subsystems
	SDL_Quit();
}