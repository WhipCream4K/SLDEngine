#include "OnBulletContact.h"
#include "MyComponents.h"
#include "GalagaScene.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include <Components/TransformComponent.h>

OnBulletContact::OnBulletContact(SLD::WorldEntity& world)
	: PhysicsCallback(world)
{
}

void OnBulletContact::OnBeginOverlap(SLD::GameObjectId collider, SLD::GameObjectId against, b2Contact* )
{
	using namespace SLD;
	
	ProjectileComponent* projectile{ m_World.GetComponent<ProjectileComponent>(collider) };

	if (!projectile->isActive)
		return;
	
	HealthComponent* hPtr{ m_World.GetComponent<HealthComponent>(against) };
	if(hPtr)
	{
		std::cout << "Bullet Contact against id : " << against << '\n';
		std::cout << "Current Health : " << --hPtr->health << '\n';

		projectile->isActive = false;

		if (hPtr->health <= 0)
		{
			const auto& worldPos{ m_World.GetComponent<TransformComponent>(against)->GetWorldPos() };

			Instance<ParticleManager>()->Spawn(m_World, worldPos, ParticleType::EnemyDead);

			m_World.DestroyGameObject(against);

		}
	}
}
