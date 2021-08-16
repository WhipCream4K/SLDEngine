#include "UpdateFlyInPath.h"

UpdateFlyInPath::UpdateFlyInPath(SLD::WorldEntity& world)
	: SystemTemplate(world, SLD::PipelineLayer::OnFixedUpdate)
{
}

void UpdateFlyInPath::OnUpdate(SLD::GameObjectId , float deltaTime, SLD::TransformComponent* transform,
	FlyInComponent* flyIn, SLD::Box2DComponent* box2d, SpeedComponent* speed)
{
	using namespace rtm;
	using namespace SLD;
	
	if (size_t(flyIn->currentWayPoint) < flyIn->carryPath.size())
	{
		const auto& worldPos{ transform->GetWorldPos() };
		auto& path{ flyIn->carryPath };

		auto startP{ vector_load2(&path[flyIn->currentWayPoint].x) };
		const auto currentP{ vector_load2(&worldPos.x) };

		if (vector_length_squared(vector_sub(startP, currentP)) <= 5.0f)
		{
			flyIn->currentWayPoint++;
		}

		if (size_t(flyIn->currentWayPoint) < flyIn->carryPath.size())
		{
			startP = vector_load2(&path[flyIn->currentWayPoint]);
			auto vecToP{ vector_sub(startP ,currentP) };

			float2f slideVec{};
			vecToP = vector_normalize3(vecToP);
			vector_store2(vecToP, &slideVec);

			box2d->GetBody()->SetLinearVelocity({ slideVec.x * deltaTime * speed->value,slideVec.y * deltaTime * speed->value });
		}
		else
		{
			box2d->GetBody()->SetLinearVelocity({ 0.0f,0.f });
			//m_World.DestroyComponent<FlyInComponent>(gId);
		}

	}
}

