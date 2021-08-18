#pragma once

#include <System/SystemTemplate.h>
#include <Components/TransformComponent.h>
#include <Physics/Components/Box2DComponent.h>
#include "MyComponents.h"

class EnemyFlyInSystem : public SLD::SystemTemplate<
	SLD::TransformComponent,
	SLD::Box2DComponent,
	FlyInComponent>
{
	
};

