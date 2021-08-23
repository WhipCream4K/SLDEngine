#include "OnObjectHit.h"
#include "ParticleManager.h"
#include <Components/TransformComponent.h>
#include "EnemyManager.h"
#include "MyComponents.h"
#include "OnPlayerDead.h"
#include "OnEnemyDead.h"
#include "ResourceManager.h"

void PlayerOnHit::OnHit(SLD::WorldEntity& world, SLD::GameObjectId id, bool isDead)
{
	using namespace SLD;

	if(isDead)
	{
		TransformComponent* transform{ world.GetComponent<TransformComponent>(id) };
		Instance<ParticleManager>()->Spawn(world, transform->GetWorldPos(), ParticleType::PlayerDead);
		world.BroadCast<OnPlayerDead>(id,isDead);
	}
}

void EnemyHit::OnHit(SLD::WorldEntity& world, SLD::GameObjectId id, bool isDead)
{
	using namespace SLD;
	
	if(isDead)
	{
		TransformComponent* transform{ world.GetComponent<TransformComponent>(id) };
		Instance<ParticleManager>()->Spawn(world, transform->GetWorldPos(), ParticleType::EnemyDead);

		// Reset Health
		HealthComponent* health{ world.GetComponent<HealthComponent>(id) };
		if(health)
		{
			health->health = health->maxHealth;
		}

		// Call Enemy manager to hide this
		Instance<EnemyManager>()->Hide(world, id);
		world.BroadCast<OnEnemyDead>(id);
	}

}

void BossHit::OnHit(SLD::WorldEntity& world, SLD::GameObjectId id, bool isDead)
{
	using namespace SLD;
	
	if(isDead)
	{
		EnemyHit::OnHit(world, id, isDead);
	}
	else
	{
		EnemyTag* tag{ world.GetComponent<EnemyTag>(id) };
		if(tag)
		{
			if(tag->type == EnemyType::Galagas)
			{
				// Change sprite to purple
				sf::Texture* purpleGalagas{ Instance<ResourceManager>()->Get<sf::Texture>("GalagasPurple") };
				if (purpleGalagas)
				{
					SpriteRenderComponent* sprite{ world.GetComponent<SpriteRenderComponent>(id) };
					if (sprite)
					{
						sprite->sprite.setTexture(*purpleGalagas);
					}
				}
			}
		}
	}
}
