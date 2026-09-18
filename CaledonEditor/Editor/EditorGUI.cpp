/*------------------------------
| File: EditorGUI.cpp
| Author: Chandler Mays
------------------------------*/
#include "EditorGUI.h"

#include "CaledonEngine/Systems/Engine/EngineManager.h"
#include "CaledonEngine/Systems/Rendering/GraphicsManager.h"
#include "CaledonEngine/Systems/Rendering/Window.h"
#include "CaledonEngine/Systems/Rendering/Renderer.h"

#include <SDL3/SDL.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_internal.h>
#include <ImGUI/imgui_impl_sdl3.h>
#include <ImGUI/imgui_impl_sdlrenderer3.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-------------------------------------------------------------------
| --- Constructor: Constructs the EditorGUI with default values --- |
-------------------------------------------------------------------*/
EditorGUI::EditorGUI()
	: m_isInitialized{ false }
    , m_isLayoutInitialized{ false }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
EditorGUI::~EditorGUI()
{
	Shutdown();
}

/*----------------------------------------------------
| --- Initialize: Prepares the EditorGUI for use --- |
----------------------------------------------------*/
bool EditorGUI::Initialize()
{
    CE::GraphicsManager* graphics = CE::EngineManager::GetInstance().GetGraphicsManager();
    CE::Window* window = graphics ? graphics->GetWindow() : nullptr;
    CE::Renderer* renderer = graphics ? graphics->GetRenderer() : nullptr;

    SDL_Window* sdlWindow = window ? static_cast<SDL_Window*>(window->GetNativeHandle()) : nullptr;
    SDL_Renderer* sdlRenderer = renderer ? static_cast<SDL_Renderer*>(renderer->GetNativeHandle()) : nullptr;

    if (!sdlWindow || !sdlRenderer)
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForSDLRenderer(sdlWindow, sdlRenderer))
    {
        ImGui::DestroyContext();
        return false;
    }

    if (!ImGui_ImplSDLRenderer3_Init(sdlRenderer))
    {
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        return false;
    }

    m_isInitialized = true;
    return true;
}

/*-------------------------------------------------------------------
| --- BeginFrame: 
-------------------------------------------------------------------*/
void EditorGUI::BeginFrame()
{
    if (!m_isInitialized)
        return;

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

/*-------------------------------------------------------------------
| --- EndFrame:
-------------------------------------------------------------------*/
void EditorGUI::EndFrame()
{
    if (!m_isInitialized)
        return;

    ImGui::Render();

    CE::GraphicsManager* pGraphicsMgr = CE::EngineManager::GetInstance().GetGraphicsManager();
    CE::Renderer* pRenderer = pGraphicsMgr ? pGraphicsMgr->GetRenderer() : nullptr;
    SDL_Renderer* pSDLRenderer = pRenderer ? static_cast<SDL_Renderer*>(pRenderer->GetNativeHandle()) : nullptr;

    if (pSDLRenderer)
    {
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), pSDLRenderer);
    }
}

/*-------------------------------------------------------------------
| --- ProcessEvent: 
-------------------------------------------------------------------*/
void EditorGUI::ProcessEvent(const void* pEvent)
{
    if (!m_isInitialized || !pEvent)
        return;

    ImGui_ImplSDL3_ProcessEvent(static_cast<const SDL_Event*>(pEvent));
}

/*-------------------------------------------------------------------
| --- Shutdown:
-------------------------------------------------------------------*/
void EditorGUI::Shutdown()
{
    if (!m_isInitialized)
        return;

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    m_isInitialized = false;
}

/*-------------------------------------------------------------------
| --- BeginDockspace:
-------------------------------------------------------------------*/
void EditorGUI::BeginDockspace()
{
    if (!m_isInitialized)
        return;

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags hostFlags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("EditorDockspaceHost", nullptr, hostFlags);

    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("CaledonEditorDockspace");

    ImGuiDockNodeFlags dockspaceFlags =
        ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::DockSpace(
        dockspaceId,
        ImVec2(0.0f, 0.0f),
        dockspaceFlags);

    if (!m_isLayoutInitialized)
    {
        BuildDefaultLayout();
        m_isLayoutInitialized = true;
    }
}

/*-------------------------------------------------------------------
| --- EndDockspace:
-------------------------------------------------------------------*/
void EditorGUI::EndDockspace()
{
    if (!m_isInitialized)
        return;

    ImGui::End();
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*-------------------------------------------------------------------
| --- BuildDefaultLayout: 
-------------------------------------------------------------------*/
void EditorGUI::BuildDefaultLayout()
{
    ImGuiID dockspaceId = ImGui::GetID("CaledonEditorDockspace");

    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(
        dockspaceId,
        ImGuiDockNodeFlags_DockSpace);

    ImGui::DockBuilderSetNodeSize(
        dockspaceId,
        ImGui::GetMainViewport()->WorkSize);

    ImGuiID leftDockId = 0;
    ImGuiID rightDockId = 0;
    ImGuiID bottomDockId = 0;
    ImGuiID centerDockId = dockspaceId;

    leftDockId = ImGui::DockBuilderSplitNode(
        centerDockId,
        ImGuiDir_Left,
        0.20f,
        nullptr,
        &centerDockId);

    rightDockId = ImGui::DockBuilderSplitNode(
        centerDockId,
        ImGuiDir_Right,
        0.22f,
        nullptr,
        &centerDockId);

    bottomDockId = ImGui::DockBuilderSplitNode(
        centerDockId,
        ImGuiDir_Down,
        0.20f,
        nullptr,
        &centerDockId);

    ImGui::DockBuilderDockWindow("Hierarchy", leftDockId);
    ImGui::DockBuilderDockWindow("Inspector", rightDockId);
    ImGui::DockBuilderDockWindow("Project", bottomDockId);
    ImGui::DockBuilderDockWindow("Viewport", centerDockId);

    ImGui::DockBuilderFinish(dockspaceId);
}