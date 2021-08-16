#pragma once

#include <SLDFramework.h>
#include <System/SystemTemplate.h>
#include "MyComponents.h"

class UpdateParticle : public SLD::SystemTemplate<SLD::TransformComponent,ParticleComponent>
{
public:

	UpdateParticle(SLDWorldEntity& world);
	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, ParticleComponent*) override;

private:

	std::unordered_map<SLD::GameObjectId, float> m_Timers;
};

