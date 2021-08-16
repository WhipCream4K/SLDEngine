#include "PhysicsToTransformUpdate.h"
#include <box2d/box2d.h>

SLD::PhysicsToTransformUpdate::PhysicsToTransformUpdate(WorldEntity& world)
	: SystemTemplate(world, PipelineLayer::OnPostPhysicsStep)
{
}

void SLD::PhysicsToTransformUpdate::OnUpdate(GameObjectId, float, TransformComponent* transform,
	Box2DComponent* box2d)
{
	if(box2d)
	{
		const auto& body{ box2d->GetBody() };
		if(body)
		{
			const auto& [x, y] {box2d->GetBody()->GetPosition()};
			const auto& lastFramePos{ transform->GetWorldPos() };

			transform->Translate(x, y, lastFramePos.z);
		}

	}

}
