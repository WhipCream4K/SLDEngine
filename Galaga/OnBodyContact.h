#pragma once

#include <Physics/System/PhysicsCallback.h>
#include <Physics/Components/Box2DComponent.h>

class OnBodyContact : public SLD::PhysicsCallback<SLD::Box2DComponent>
{
public:

	OnBodyContact(SLD::WorldEntity& world);
	void OnBeginOverlap(SLD::GameObjectId collider, SLD::GameObjectId against, b2Contact* contactPoint) override;
	void OnUpdate(SLD::GameObjectId , float , SLD::TransformComponent*, SLD::Box2DComponent*, SLD::Box2DComponent*) override {}

private:

	SLD::GameObjectId m_Contact{};
	SLD::GameObjectId m_Against{};
};

