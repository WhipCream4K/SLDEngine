#include "WorldToPhysicsUpdate.h"

SLD::WorldToPhysicsUpdate::WorldToPhysicsUpdate(WorldEntity& world)
	: SystemTemplate(world, PipelineLayer::OnPrePhysicsStep)
{
}

void SLD::WorldToPhysicsUpdate::OnUpdate(GameObjectId, float, TransformComponent*,
	Box2DComponent* box2d)
{
	if (box2d)
	{
		if (box2d->GetState() == Box2DComponent::State::Teleport)
		{
			const auto& pos{ box2d->GetPosition() };
			const auto& rigidBody{ box2d->GetBody() };

			const float angle{ rigidBody->GetAngle() };
			rigidBody->SetTransform({ pos.x,pos.y }, angle);
		}
	}
}
