#pragma once

#include <SLDFramework.h>

#include "CollisionGroup.h"

class ShootableComponent : SLD::ComponentT<ShootableComponent>
{
public:

	ShootableComponent(CollisionGroup group, float reloadTime);
	ShootableComponent(CollisionGroup group, float reloadTime, int magazine);
	
	bool Shoot(
		SLD::WorldEntity& world,
		const rtm::float3f& from,
		const rtm::float2f& dir);

	bool ShouldReload() const;
	float GetReloadTime() const;
	void SetReloadFinished();
	int GetShotsCount() const;

private:

	std::vector<SLD::GameObjectId> m_Magazine{};
	float m_ReloadTime{};
	int m_ShotsTracker{};
	CollisionGroup m_ShooterGroup;
	bool m_ShouldReload{};
	bool m_UseMagazine{};
};

