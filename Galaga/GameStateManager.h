#pragma once

#include <SLDFramework.h>
#include <Core/Singleton.h>
#include "GameStateComponent.h"

constexpr int MAX_PLAYER{ 2 };

class GameStateManager : public SLD::Singleton<GameStateManager>
{
public:

	friend class Singleton<GameStateManager>;

	SLD::GameObjectId SpawnGameState(SLD::WorldEntity& world,bool startActive);
	GameStateComponent* GetActiveGameState(SLD::WorldEntity& world);

private:

	std::array<SLD::GameObjectId, MAX_PLAYER> m_GameStates;
};