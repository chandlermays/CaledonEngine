/*------------------------------
| File: PlayerController.cpp
| Author: Chandler Mays
------------------------------*/
#include "PlayerController.h"

#include "CaledonEngine/Core/GameObject.h"
#include "CaledonEngine/Core/Transform.h"

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

    if (m_horizontalInput != 0.0f || m_verticalInput != 0.0f)
    {
        float horizontal = m_horizontalInput;
        float vertical = m_verticalInput;

        // Clamp the combined input vector's magnitude to 1 so diagonal movement isn't faster than moving along a single axis.
        float magnitude = std::sqrt(horizontal * horizontal + vertical * vertical);
        if (magnitude > 1.0f)
        {
            horizontal /= magnitude;
            vertical /= magnitude;
        }

        float moveX = horizontal * m_moveSpeed * deltaTime;
        float moveY = -vertical * m_moveSpeed * deltaTime;

        CE::Transform& transform = m_pOwner->GetTransform();
        CE::Vector2f currentPosition = transform.GetPosition();

        currentPosition.x += moveX;
        currentPosition.y += moveY;

        transform.SetPosition(currentPosition);
    }
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