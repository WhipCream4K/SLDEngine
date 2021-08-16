#pragma once

#include <SLDFramework.h>
#include <System/SystemTemplate.h>
#include <Components/TransformComponent.h>
#include "MyComponents.h"

class LimitPlayerToPlayArea : public SLD::SystemTemplate<SLD::TransformComponent, PlayerTag>
{
public:
	
	LimitPlayerToPlayArea(SLD::WorldEntity& world, const rtm::float4f& playArea);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, PlayerTag*) override;

private:

	rtm::float4f m_AreaRect;
	float m_Right{};
	float m_Left{};
	float m_Up{};
	float m_Down{};
};

