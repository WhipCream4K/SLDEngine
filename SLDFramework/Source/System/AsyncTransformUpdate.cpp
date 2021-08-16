#include "AsyncTransformUpdate.h"

SLD::AsyncTransformUpdate::AsyncTransformUpdate(WorldEntity& world)
	: SystemTemplate(world,PipelineLayer::OnPostAsync)
{
}

void SLD::AsyncTransformUpdate::OnUpdate(GameObjectId , float , TransformComponent* transform)
{
	transform->GetWorldMatrix();
}
