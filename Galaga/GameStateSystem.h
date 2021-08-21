#pragma once

#include <System/SystemTemplate.h>
#include "GameStateComponent.h"
#include "MyComponents.h"
#include "OnPlayerDead.h"

class Player;
class GameStateSystem : public SLD::SystemTemplate<
	GameStateComponent,
	InputListener>
{
public:
	
	
	GameStateSystem(SLD::WorldEntity& world);
	GameStateSystem(SLD::WorldEntity& world, SLD::GameObjectId gameStateObject);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, GameStateComponent*, InputListener*) override;
	void PlayerDead(SLD::GameObjectId playerId,bool);
	//void OnReceieveMessage(SLD::GameObjectId id, GameStateComponent*, InputListener*);

private:

	void HandleMenuState(GameStateComponent* game);
	void HandlePlayState(GameStateComponent* game);

	SharedPtr<OnPlayerDead> OnPlayerDead;

	SLD::GameObjectId m_GameStateObject{};
	//std::array<bool, size_t(GameStateNum::Count)> m_StateBits;
	//std::array<std::vector<WeakPtr<SystemBase>>, size_t(GameStateNum::Count)> m_SystemStateArray;
	//size_t m_PlayerCount;
};

