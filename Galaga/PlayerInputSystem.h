#pragma once

#include <SLDFramework.h>
#include <Components/TransformComponent.h>
#include <Physics/Components/Box2DComponent.h>
#include "MyComponents.h"
#include "System/SystemTemplate.h"

class PlayerInputSystem :
	public SLD::SystemTemplate<
	SLD::TransformComponent,
	InputListener,
	SLD::Box2DComponent,
	SpeedComponent>
{
public:

	PlayerInputSystem(SLD::WorldEntity& world);

	//void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, InputListener*, SLD::Box2DComponent*) override;
	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, InputListener*, SLD::Box2DComponent*, SpeedComponent*) override;

private:

	int m_FrameCounter{};
};

