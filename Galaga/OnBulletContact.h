#pragma once

#include <Physics/System/PhysicsCallback.h>
#include "MyComponents.h"

class OnBulletContact : public SLD::PhysicsCallback<ProjectileComponent>
{
public:

	OnBulletContact(SLD::WorldEntity& world);
	void OnBeginOverlap(SLD::GameObjectId collider, SLD::GameObjectId aginst, b2Contact* contactPoint) override;
	void OnUpdate(SLD::GameObjectId , float , SLD::TransformComponent*, SLD::Box2DComponent*, ProjectileComponent*) override{}
};

