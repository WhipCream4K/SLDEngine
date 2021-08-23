#include "ShootableComponent.h"
#include "BulletManager.h"
#include "MyComponents.h"

ShootableComponent::ShootableComponent(CollisionGroup group, float reloadTime)
	: m_ReloadTime(reloadTime)
	, m_ShooterGroup(group)
	, m_ShouldReload()
{
}

ShootableComponent::ShootableComponent(CollisionGroup group, float reloadTime, int magazine)
	: m_ReloadTime(reloadTime)
	, m_ShooterGroup(group)
	, m_UseMagazine(true)
{
	m_Magazine.resize(magazine);
}

bool ShootableComponent::Shoot(SLD::WorldEntity& world, const rtm::float3f& from, const rtm::float2f& dir)
{
	using namespace SLD;

	bool shotValid{};
	if (!m_ShouldReload)
	{
		if (m_UseMagazine)
		{
			for (auto& bulletId : m_Magazine)
			{
				if (bulletId != 0)
				{
					ProjectileComponent* projComp{ world.GetComponent<ProjectileComponent>(bulletId) };
					if (projComp)
					{
						if (!projComp->isActive)
						{
 							bulletId = Instance<BulletManager>()->Spawn(world, from, dir, m_ShooterGroup);
							shotValid = true;
							break;
						}
					}
				}
				else
				{
					bulletId = Instance<BulletManager>()->Spawn(world, from, dir, m_ShooterGroup);
					shotValid = true;
					break;
				}
			}


		}
		else
		{
			Instance<BulletManager>()->Spawn(world, from, dir, m_ShooterGroup);
			shotValid = true;
		}
		
		m_ShotsTracker++;
		m_ShouldReload = true;
	}

	return shotValid;
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

int ShootableComponent::GetShotsCount() const
{
	return m_ShotsTracker;
}
