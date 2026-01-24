#include "PlayerController.h"
#include "CaledonEngine/Core/GameObject.h"

void PlayerController::ConfigureInputBindings()
{
    if (!m_pGameplayActionMap)
        return;

    CE::InputAction* pMoveHorizontal = m_pGameplayActionMap->GetActionByName("MoveHorizontal");
    CE::InputAction* pMoveVertical = m_pGameplayActionMap->GetActionByName("MoveVertical");

    if (pMoveHorizontal)
    {
        pMoveHorizontal->OnValue([this](float value) { OnMoveHorizontal(value); });
    }

    if (pMoveVertical)
    {
        pMoveVertical->OnValue([this](float value) { OnMoveVertical(value); });
    }
}

PlayerController::PlayerController()
    : CE::Component()
    , m_pInputActions{ nullptr }
    , m_pGameplayActionMap{ nullptr }
    , m_moveSpeed{ 100.0f }
    , m_horizontalInput{ 0.0f }
	, m_verticalInput{ 0.0f }
{ }

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

void PlayerController::Update(float deltaTime)
{
    if (!m_pOwner)
        return;

    if (m_horizontalInput != 0.0f || m_verticalInput != 0.0f)
    {
        float moveX = m_horizontalInput * m_moveSpeed * deltaTime;
        float moveY = -m_verticalInput * m_moveSpeed * deltaTime;

        CE::Transform& transform = m_pOwner->GetTransform();
        CE::VectorFloat currentPosition = transform.GetPosition();

        currentPosition.m_x += moveX;
        currentPosition.m_y += moveY;

        transform.SetPosition(currentPosition);
    }
}

void PlayerController::SetInputActions(GameInputActions* pInputActions)
{
    m_pInputActions = pInputActions;
}

void PlayerController::OnMoveHorizontal(float value)
{
    m_horizontalInput = value;
}

void PlayerController::OnMoveVertical(float value)
{
    m_verticalInput = value;
}