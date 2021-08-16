#pragma once

#include <SLDFramework.h>

#include "CollisionGroup.h"

class ShootableComponent : SLD::ComponentT<ShootableComponent>
{
public:

	ShootableComponent(CollisionGroup group, float reloadTime);
	
	void Shoot(
		SLD::WorldEntity& world,
		const rtm::float3f& from,
		const rtm::float2f& dir);

	bool ShouldReload() const;
	float GetReloadTime() const;
	void SetReloadFinished();

private:
	
	float m_ReloadTime{};
	bool m_ShouldReload{};
	CollisionGroup m_ShooterGroup;
};

