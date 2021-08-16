#include "UpdateParticle.h"
#include "GalagaScene.h"
#include <Components/TransformComponent.h>

UpdateParticle::UpdateParticle(SLDWorldEntity& world)
	: SystemTemplate(world,SLD::PipelineLayer::OnPreAsync)
{
}

void UpdateParticle::OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent* transform,
                              ParticleComponent* particle)
{
	if(particle->isActive)
	{
		float& counter{ m_Timers[gameObjectId] };
		counter += deltaTime;
		if (counter >= particle->lifeTime)
		{
			particle->isActive = false;
			// HIDE
			transform->Translate(0.0f, 1000.0f, float(GalagaScene::Layer::Particle));
			counter = 0.0f;
		}
	}
}
