#include "ShootableComponent.h"
#include "BulletManager.h"

ShootableComponent::ShootableComponent(CollisionGroup group, float reloadTime)
	: m_ReloadTime(reloadTime)
	, m_ShouldReload()
	, m_ShooterGroup(group)
{
}

void ShootableComponent::Shoot(SLD::WorldEntity& world, const rtm::float3f& from, const rtm::float2f& dir)
{
	using namespace SLD;

	if (!m_ShouldReload)
	{
		Instance<BulletManager>()->Spawn(world, from, dir,m_ShooterGroup);
		m_ShouldReload = true;
	}
}

bool ShootableComponent::ShouldReload() const
{
	return m_ShouldReload;
}

float ShootableComponent::GetReloadTime() const
{
	return m_ReloadTime;
}

void ShootableComponent::SetReloadFinished()
{
	m_ShouldReload = false;
}
