#include "OnBodyContact.h"
#include "MyComponents.h"

OnBodyContact::OnBodyContact(SLD::WorldEntity& world)
	: PhysicsCallback(world)
{
}

void OnBodyContact::OnBeginOverlap(SLD::GameObjectId collider, SLD::GameObjectId against, b2Contact* )
{

	if (m_Contact == collider || m_Contact == against)
		return;
	if (m_Against == against || m_Against == collider)
		return;

	m_Against = against;
	m_Contact = collider;
	
	HealthComponent* colliderHealth{ m_World.GetComponent<HealthComponent>(collider) };
	if(colliderHealth)
	{
		colliderHealth->health--;
		const bool isDead{ colliderHealth->health <= 0 ? true : false };
		OnHitCommand* hitInterface{ m_World.GetComponent<OnHitCommand>(collider) };
		if (hitInterface)
		{
			hitInterface->onHit->OnHit(m_World, collider, isDead);
		}
	}

	HealthComponent* healthComp{ m_World.GetComponent<HealthComponent>(against) };
	if (healthComp)
	{
#ifdef _DEBUG
		std::cout << "Contact against id : " << against << '\n';
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
}
