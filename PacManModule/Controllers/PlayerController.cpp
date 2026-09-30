/*------------------------------
| File: PlayerController.cpp
| Author: Chandler Mays
------------------------------*/
#include "PlayerController.h"

#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Systems/Physics/Components/RigidBody2D.h"

#include <cmath>

namespace
{
    constexpr int kSlideIterations = 4;		// Contact resolutions per Slide; 2+ lets the player slide around a corner instead of stopping at it
}

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*--------------------------------------------------------------------------
| --- Constructor: Constructs the PlayerController with default values --- |
--------------------------------------------------------------------------*/
PlayerController::PlayerController()
    : CE::Component()
    , m_pInputActions{ nullptr }
    , m_pGameplayActionMap{ nullptr }
    , m_moveSpeed{ 100.0f }
    , m_horizontalInput{ 0.0f }
    , m_verticalInput{ 0.0f }
    , m_hasWarnedMissingBody{ false }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
PlayerController::~PlayerController()
{
    for (CE::InputAction* pAction : m_subscribedActions)
    {
        if (pAction)
        {
            pAction->Unsubscribe(this);
        }
    }
}

/*-----------------------------------------------------------
| --- Initialize: Prepares the PlayerController for use --- |
-----------------------------------------------------------*/
bool PlayerController::Initialize()
{
    if (m_pInputActions)
    {
        m_pGameplayActionMap = m_pInputActions->GetGameplayActionMap();
        ConfigureInputBindings();
        return true;
    }

    return false;
}

/*------------------------------------------------------------------------
| --- Update: Updates the PlayerController each frame based on input --- |
------------------------------------------------------------------------*/
void PlayerController::Update(float deltaTime)
{
    if (!m_pOwner)
        return;

    if (m_horizontalInput == 0.0f && m_verticalInput == 0.0f)
        return;

    float horizontal = m_horizontalInput;
    float vertical = m_verticalInput;

    // Clamp the combined input vector's magnitude to 1 so diagonal movement isn't faster than moving along a single axis.
    float magnitude = std::sqrt(horizontal * horizontal + vertical * vertical);
    if (magnitude > 1.0f)
    {
        horizontal /= magnitude;
        vertical /= magnitude;
    }

    // Screen-space Y points down, so positive vertical input moves toward negative Y.
    const CE::Vector2f velocity{ horizontal * m_moveSpeed, -vertical * m_moveSpeed };

    CE::RigidBody2D* pBody = GetComponent<CE::RigidBody2D>();
    if (!pBody)
    {
        // No RigidBody2D on this GameObject, so there is nothing to slide with: move freely (no collision) and say so once.
        if (!m_hasWarnedMissingBody)
        {
            CE_LOG("PlayerController::Update - '{}' has no RigidBody2D; moving without collision.", m_pOwner->GetName());
            m_hasWarnedMissingBody = true;
        }

        CE::Transform& transform = m_pOwner->GetTransform();
        transform.SetPosition(transform.GetPosition() + velocity * deltaTime);
        return;
    }

    // Slide only calculates the target position; applying it is our job.
    CE::SlideConfig2D config;
    config.startPosition = pBody->GetPosition();
    config.maxIterations = kSlideIterations;

    const CE::SlideResults2D results = pBody->Slide(velocity, deltaTime, config);
    pBody->SetPosition(results.position);
}

/*-------------------------------------------------------------------------------
| --- SetInputActions: Assigns the GameInputActions to the PlayerController --- |
-------------------------------------------------------------------------------*/
void PlayerController::SetInputActions(GameInputActions* pInputActions)
{
    m_pInputActions = pInputActions;
}

/*-------------------------------------------------------------
| --- OnMoveHorizontal: Handles horizontal movement input --- |
-------------------------------------------------------------*/
void PlayerController::OnMoveHorizontal(float value)
{
    m_horizontalInput = value;
}

/*---------------------------------------------------------
| --- OnMoveVertical: Handles vertical movement input --- |
---------------------------------------------------------*/
void PlayerController::OnMoveVertical(float value)
{
    m_verticalInput = value;
}

/*-------------------------------------------------------------
| --- GetTypeName: Returns the type name of the Component --- |
-------------------------------------------------------------*/
const std::string& PlayerController::GetTypeName() const
{
    static const std::string typeName = "PlayerController";
    return typeName;
}

/*-------------------------------------------------------------
| --- SetMoveSpeed: Sets the movement speed of the player --- |
-------------------------------------------------------------*/
void PlayerController::SetMoveSpeed(float speed)
{
    m_moveSpeed = speed;
}

/*----------------------------------------------------------------
| --- GetMoveSpeed: Returns the movement speed of the player --- |
----------------------------------------------------------------*/
float PlayerController::GetMoveSpeed() const
{
    return m_moveSpeed;
}


/*------------------------------------
| --- Private Method Definitions --- |
------------------------------------*/
/*------------------------------------------------------------------------------
| --- ConfigureInputBindings: Sets up input action bindings for the player --- |
------------------------------------------------------------------------------*/
void PlayerController::ConfigureInputBindings()
{
    if (!m_pGameplayActionMap)
        return;

    CE::InputAction* pMoveHorizontal = m_pGameplayActionMap->GetActionByName("MoveHorizontal");
    CE::InputAction* pMoveVertical = m_pGameplayActionMap->GetActionByName("MoveVertical");

    if (pMoveHorizontal)
    {
        pMoveHorizontal->OnValue(this, &PlayerController::OnMoveHorizontal);
        m_subscribedActions.emplace_back(pMoveHorizontal);
    }

    if (pMoveVertical)
    {
        pMoveVertical->OnValue(this, &PlayerController::OnMoveVertical);
        m_subscribedActions.emplace_back(pMoveVertical);
    }
}