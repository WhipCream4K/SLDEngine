#pragma once

#include <SLDFramework.h>
#include <System/SystemTemplate.h>
#include "GameStateComponent.h"

class OnEnemyDead;
class UpdateScore : public SLD::SystemTemplate<GameStateComponent>
{
public:

	UpdateScore(SLDWorldEntity& world);
	void OnUpdate(SLD::GameObjectId , float , GameStateComponent*) override {}

	void EnemyDead(SLD::GameObjectId id);
	
private:

	SharedPtr<OnEnemyDead> m_OnEnemyDead;
};

