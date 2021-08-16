#pragma once

#include <System/SystemTemplate.h>
#include <Components/TransformComponent.h>
#include <Physics/Components/Box2DComponent.h>
#include "MyComponents.h"

class UpdateFlyInPath : public SLD::SystemTemplate<
	SLD::TransformComponent,
	FlyInComponent,
	SLD::Box2DComponent,
	SpeedComponent>
{
public:

	UpdateFlyInPath(SLD::WorldEntity& world);
	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, FlyInComponent*, SLD::Box2DComponent*, SpeedComponent*) override;
};

