#include "OnBulletContact.h"
#include "MyComponents.h"
#include "GalagaScene.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include "OnObjectHit.h"
#include <Components/TransformComponent.h>

OnBulletContact::OnBulletContact(SLD::WorldEntity& world)
	: PhysicsCallback(world)
{
}

void OnBulletContact::OnBeginOverlap(SLD::GameObjectId collider, SLD::GameObjectId against, b2Contact*)
{
	using namespace SLD;

	ProjectileComponent* projectile{ m_World.GetComponent<ProjectileComponent>(collider) };

	if (!projectile->isActive)
		return;

	HealthComponent* healthComp{ m_World.GetComponent<HealthComponent>(against) };
	if (healthComp)
	{
#ifdef _DEBUG
		std::cout << "Bullet Contact against id : " << against << '\n';
		std::cout << "Current Health : " << healthComp->health << '\n';
#endif

		healthComp->health--;
		const bool isDead{ healthComp->health <= 0 ? true : false };
		OnHitCommand* hitInterface{ m_World.GetComponent<OnHitCommand>(against) };
		if (hitInterface)
		{
			hitInterface->onHit->OnHit(m_World, against, isDead);
		}
	}

	projectile->isActive = false;

	//OnObjectHit* hitInterface{ m_World.GetComponent<OnObjectHit>(against) };
	//if(hitInterface)
	//{
	//	//hitInterface->OnHit();
	//}
	//HealthComponent* hPtr{ m_World.GetComponent<HealthComponent>(against) };
	//if(hPtr)
	//{
	//	std::cout << "Bullet Contact against id : " << against << '\n';
	//	std::cout << "Current Health : " << --hPtr->health << '\n';

	//	projectile->isActive = false;

	//	if (hPtr->health <= 0)
	//	{
	//		const auto& worldPos{ m_World.GetComponent<TransformComponent>(against)->GetWorldPos() };

	//		Instance<ParticleManager>()->Spawn(m_World, worldPos, ParticleType::EnemyDead);

	//		m_World.DestroyGameObject(against);

	//	}
	//}
}
