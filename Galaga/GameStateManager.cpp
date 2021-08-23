#include "GameStateManager.h"

SLD::GameObjectId GameStateManager::SpawnGameState(SLD::WorldEntity& world, bool startActive)
{
	
}

GameStateComponent* GameStateManager::GetActiveGameState(SLD::WorldEntity& world)
{
	for (auto id : m_GameStates)
	{
		GameStateComponent* gameState{ world.GetComponent<GameStateComponent>(id) };
		if (gameState->isActive)
			return gameState;
	}

	return nullptr;
}
