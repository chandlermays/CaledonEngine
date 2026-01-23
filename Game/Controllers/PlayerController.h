#pragma once
#include "CaledonEngine/Core/Component.h"
#include "Game/Input/GameInputActions.h"

class PlayerController : public CE::Component
{
private:
	GameInputActions* m_pInputActions;
	CE::InputActionMap* m_pGameplayActionMap;
	
	float m_moveSpeed;
	float m_horizontalInput;
	float m_verticalInput;

	void ConfigureInputBindings();

public:
	PlayerController();
	~PlayerController() override = default;
	PlayerController(const PlayerController&) = delete;
	PlayerController& operator=(const PlayerController&) = delete;
	PlayerController(PlayerController&&) = delete;
	PlayerController& operator=(PlayerController&&) = delete;

	bool Initialize() override;
	void Update(float deltaTime) override;

	void SetInputActions(GameInputActions* pInputActions);

	void OnMoveHorizontal(float value);
	void OnMoveVertical(float value);
};