#pragma once

#include <System/SystemTemplate.h>
#include "GameStateComponent.h"
#include "MyComponents.h"
//#include "OnPlayerDead.h"

class OnPlayerDead;
class OnPlayerShoot;
class OnEnemyDead;
class Player;
class GameStateSystem : public SLD::SystemTemplate<
	GameStateComponent>
{
public:
	
	
	GameStateSystem(SLD::WorldEntity& world);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, GameStateComponent*) override;
	void PlayerDead(SLD::GameObjectId playerId,bool);
	void EnemyDead(SLD::GameObjectId enemyId);
	void PlayerShoot(SLD::GameObjectId);

private:

	SharedPtr<OnPlayerDead> m_OnPlayerDead;
	SharedPtr<OnEnemyDead> m_OnEnemyDead;
	SharedPtr<OnPlayerShoot> m_OnPlayerShoot;
};

