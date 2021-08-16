#include "EnemyStateSystem.h"
#include <Components/TransformComponent.h>
#include "FormationWayPoints.h"

EnemyStateSystem::EnemyStateSystem(SLD::WorldEntity& world)
	: SystemTemplate(world, SLD::PipelineLayer::OnFixedUpdate)
{
	m_LineFormationWayPoints.reserve(4);
}

EnemyStateSystem::EnemyStateSystem(SLD::WorldEntity& world, SLD::GameObjectId pathId)
	: SystemTemplate(world, SLD::PipelineLayer::OnFixedUpdate)
	, m_FormationWayPointObjectId(pathId)
{
}

void EnemyStateSystem::OnUpdate(SLD::GameObjectId, float deltaTime, SLD::TransformComponent* transform,
	SLD::Box2DComponent* box2d, SpeedComponent* speed, FlyInComponent* flyIn, FormationComponent* formation, EnemyTag* tag)
{
	switch (tag->state)
	{
	case EnemyState::FlyIn:

		HandleFlyInState(deltaTime, transform, box2d, flyIn, speed, tag);
		break;

	case EnemyState::Formation:

		HandleFormationState(deltaTime, transform, box2d, formation, speed, tag);

	case EnemyState::Dive:

		
		
		break;
	}
}

void EnemyStateSystem::HandleFlyInState(float dt, SLD::TransformComponent* transform, SLD::Box2DComponent* box2d,
	FlyInComponent* flyIn, SpeedComponent* speed, EnemyTag* tag)
{
	using namespace SLD;
	using namespace rtm;

	const auto& travelPath{ flyIn->carryPath };

	if (size_t(flyIn->currentWayPoint) < travelPath.size())
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

			box2d->GetBody()->SetLinearVelocity({ slideVec.x * dt * speed->value,slideVec.y * dt * speed->value });
		}
		else
		{
			box2d->GetBody()->SetLinearVelocity({ 0.0f,0.f });
			tag->state = EnemyState::Formation;
		}

	}

}

void EnemyStateSystem::HandleFormationState(float dt, SLD::TransformComponent* transform, SLD::Box2DComponent* box2d,
	FormationComponent* formation, SpeedComponent* speed, EnemyTag*)
{
	using namespace SLD;
	using namespace rtm;

	FormationWayPoints* formationWayPoints{ m_World.GetComponent<FormationWayPoints>(m_FormationWayPointObjectId) };

	if(formationWayPoints)
	{
		const auto& wayPoints{ formationWayPoints->GetWayPoints() };
		const auto& targetSlot{ wayPoints[formation->row][formation->col] };

		if (!formation->alreadyInside)
		{
			const auto& worldPos{ transform->GetWorldPos() };
			const auto currentP{ vector_load2(&worldPos.x) };
			const auto targetP{ vector_load2(&targetSlot) };

			auto currentToTargetVec{ vector_sub(targetP,currentP) };
			if (vector_length_squared(currentToTargetVec) <= 1.0f)
			{
				formation->alreadyInside = true;
				box2d->GetBody()->SetLinearVelocity({ 0.0f,0.0f });
			}
			else
			{
				float2f slideVec{};
				currentToTargetVec = vector_normalize3(currentToTargetVec);
				vector_store2(currentToTargetVec, &slideVec);
				box2d->GetBody()->SetLinearVelocity({ slideVec.x * dt * speed->value, slideVec.y * dt * speed->value });
			}
		}
		else // Set the current transform to targetpos
		{
			box2d->GetBody()->SetTransform({ targetSlot.x,targetSlot.y }, 0.0f);
		}
	}
}

