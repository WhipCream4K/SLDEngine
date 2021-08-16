#include "ReloadShooter.h"

ReloadShooter::ReloadShooter(SLD::WorldEntity& world)
	: SystemTemplate(world,SLD::PipelineLayer::OnPreAsync)
{
}

void ReloadShooter::OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, ShootableComponent* shooter)
{
	if(shooter->ShouldReload())
	{
		float& counter{ m_Timers[gameObjectId] };
		counter += deltaTime;

		if(counter >= shooter->GetReloadTime())
		{
 			shooter->SetReloadFinished();
			counter = 0.0f;
		}
	}
}
