#include "OnBodyContact.h"

OnBodyContact::OnBodyContact(SLD::WorldEntity& world)
	: PhysicsCallback(world)
{
}

void OnBodyContact::OnBeginOverlap(SLD::GameObjectId collider, SLD::GameObjectId against, b2Contact* )
{
	collider;
	against;

	
}
