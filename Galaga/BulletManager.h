#pragma once

#include <SLDFramework.h>
#include <Core/Singleton.h>

#include "CollisionGroup.h"

constexpr size_t MaximumBullet{ 2 };

class BulletManager final : public SLD::Singleton<BulletManager>
{
public:

	friend class SLD::Singleton<BulletManager>;

	void Spawn(
		SLD::WorldEntity& world, 
		const rtm::float3f& spawnPoint,
		const rtm::float2f& shootDir);

	SLD::GameObjectId Spawn(
		SLD::WorldEntity& world,
		const rtm::float3f& spawnPoint,
		const rtm::float2f& shootDir,
		CollisionGroup shootFrom);

	void Hide(SLDWorldEntity& world,
		SLD::GameObjectId id);

private:
	
	std::array<SLD::GameObjectId, MaximumBullet> m_Magazine;
	size_t m_CurrentBulletSpawn{};
};

