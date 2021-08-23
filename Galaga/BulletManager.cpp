#include "BulletManager.h"
#include "Bullet.h"
#include "MyComponents.h"
#include <Physics/Components/Box2DComponent.h>
#include <Helpers/utils.h>

#include "GalagaScene.h"

void BulletManager::Spawn(SLD::WorldEntity& world, const rtm::float3f& spawnPoint, const rtm::float2f& shootDir)
{
	using namespace SLD;

	GameObjectId bulletId{};

	bulletId = m_Magazine[m_CurrentBulletSpawn % MaximumBullet];
	
	if (bulletId == 0)
	{
		bulletId = InstantiatePrefab<Bullet>(world, {}, spawnPoint);
		m_Magazine[m_CurrentBulletSpawn] = bulletId;
	}

	Box2DComponent* box{ world.GetComponent<Box2DComponent>(bulletId) };
	ProjectileComponent* projectile{ world.GetComponent<ProjectileComponent>(bulletId) };

	if (box && projectile)
	{
		const auto physicsBody{ box->GetBody() };

		if (!physicsBody->IsAwake())
			physicsBody->SetAwake(true);

		physicsBody->SetTransform({ spawnPoint.x,spawnPoint.y }, 0.0f);
		physicsBody->SetLinearVelocity({ shootDir.x * projectile->impulseForce,shootDir.y * projectile->impulseForce });
	}

	m_CurrentBulletSpawn++;

	if (m_CurrentBulletSpawn >= MaximumBullet)
		m_CurrentBulletSpawn = 0;
}

SLD::GameObjectId BulletManager::Spawn(SLD::WorldEntity& world, const rtm::float3f& spawnPoint, const rtm::float2f& shootDir,
	CollisionGroup shootFrom)
{
	using namespace SLD;

	GameObjectId bulletId{};

	bulletId = m_Magazine[m_CurrentBulletSpawn % MaximumBullet];
	if (bulletId == 0)
	{
		bulletId = InstantiatePrefab<Bullet>(world, {}, spawnPoint);
		m_Magazine[m_CurrentBulletSpawn] = bulletId;
	}
	
	Box2DComponent* box{ world.GetComponent<Box2DComponent>(bulletId) };
	ProjectileComponent* projectile{ world.GetComponent<ProjectileComponent>(bulletId) };

	if (box && projectile)
	{
		projectile->isActive = true;
		
		const auto& physicsBody{ box->GetBody() };

		if (!physicsBody->IsAwake())
			physicsBody->SetAwake(true);

		b2Filter newFilter{ box->GetContactFilter() };
		// won't contact the shooter
		newFilter.maskBits &= ~(CollisionGroup::Projectile | shootFrom);
		
		box->SetContactFilter(newFilter);
		physicsBody->SetTransform({ spawnPoint.x,spawnPoint.y }, 0.0f);
		physicsBody->SetLinearVelocity({ shootDir.x * projectile->impulseForce,shootDir.y * projectile->impulseForce });
		//physicsBody->ApplyLinearImpulseToCenter({ shootDir.x * projectile->impulseForce,shootDir.y * projectile->impulseForce }, true);
	}

	m_CurrentBulletSpawn++;

	if (m_CurrentBulletSpawn >= MaximumBullet)
		m_CurrentBulletSpawn = 0;

	return bulletId;
}

void BulletManager::Hide(SLDWorldEntity& world, SLD::GameObjectId id)
{
	using namespace SLD;

	Box2DComponent* box{ world.GetComponent<Box2DComponent>(id) };
	if(box)
	{
		const auto& physicsBodty{ box->GetBody() };
		//physicsBodty->SetType(b2_kinematicBody);
		physicsBodty->SetTransform({ 0.0f,1000.0f }, 0.0f);
		physicsBodty->SetAwake(false);
	}
}

//void BulletManager::Hide(SLD::WorldEntity& world, SLD::GameObjectId id)
//{
//	using namespace SLD;
//
//	Box2DComponent* box{ world.GetComponent<Box2DComponent>(id) };
//	if(box)
//	{
//		const auto physicsBodty{ box->GetBody() };
//		physicsBodty->SetType(b2_kinematicBody);
//		physicsBodty->SetTransform({ 0.0f,1000.0f }, 0.0f);
//		physicsBodty->SetAwake(false);
//		//box->GetBody()->SetType(b2_kinematicBody)
//		//box->GetBody()->SetAwake(false);
//	}
//}
