#include "ZakoDiveSystem.h"

ZakoDiveSystem::ZakoDiveSystem(SLD::WorldEntity& world)
	: SystemTemplate(world)
{
}

void ZakoDiveSystem::OnUpdate(SLD::GameObjectId , float , SLD::TransformComponent*,
	SLD::Box2DComponent*, EnemyTag* tag, ShootableComponent*)
{
	
}
