#pragma once

#include <SLDFramework.h>
#include <System/SystemTemplate.h>
#include "ShootableComponent.h"

class ReloadShooter : public SLD::SystemTemplate<ShootableComponent>
{
public:

	ReloadShooter(SLD::WorldEntity& world);
	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, ShootableComponent*) override;

private:
	
	std::unordered_map<SLD::GameObjectId, float> m_Timers;
};

