#pragma once

#include <SLDFramework.h>
#include "MyComponents.h"
#include <System/SystemTemplate.h>
#include <Components/TransformComponent.h>
#include <Physics/Components/Box2DComponent.h>

class UpdateProjectile :
	public SLD::SystemTemplate<
	SLD::TransformComponent,
	ProjectileComponent,
	SLD::Box2DComponent>
{
public:

	UpdateProjectile(SLD::WorldEntity& world,float upLimit);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, ProjectileComponent*, SLD::Box2DComponent*) override;

	float m_UpLimit{};
	
};

