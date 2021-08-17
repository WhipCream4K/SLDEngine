#pragma once

#include <System/SystemTemplate.h>
#include <Physics/Components/Box2DComponent.h>
#include <Components/TransformComponent.h>
#include "ShootableComponent.h"
#include "MyComponents.h"

class ZakoDiveSystem : public SLD::SystemTemplate<
	SLD::TransformComponent,
	SLD::Box2DComponent,
	EnemyTag,
	ShootableComponent>
{
public:

	ZakoDiveSystem(SLD::WorldEntity&);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, SLD::Box2DComponent*, EnemyTag*, ShootableComponent*) override;
};

