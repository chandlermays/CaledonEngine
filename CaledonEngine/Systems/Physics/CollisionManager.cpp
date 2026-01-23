#include "CollisionManager.h"
#include <CaledonEngine/Systems/Engine/LoggingManager.h>

CE::CollisionManager::CollisionManager()
{
}

CE::CollisionManager::~CollisionManager()
{
	CE_LOG("CollisionManager::~CollisionManager - Shutting down CollisionManager.");
	Shutdown();
}

bool CE::CollisionManager::Initialize()
{
	return true;
}

void CE::CollisionManager::Update(float deltaTime)
{
}

void CE::CollisionManager::Render()
{
}

void CE::CollisionManager::Shutdown()
{
}
