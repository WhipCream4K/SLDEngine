#include "ZakoDiveSystem.h"
#include "EnemyPath.h"
#include "FormationWayPoints.h"
#include <Random/Random.h>
#include "BezierPath.h"

ZakoDiveSystem::ZakoDiveSystem(SLD::WorldEntity& world)
	: SystemTemplate(world, SLD::PipelineLayer::OnFixedUpdate)
{
	m_ZakoDivePath.resize(2);

	// even x is +
	BezierPath path{};
	path.AddCurve(BezierCurve{ {0.0f,0.0f},{0.0f,45.0f},{60.0f,45.0f},{60.0f,0.0f} }, 15);
	path.AddCurve(BezierCurve{ {60.0f,0.0f},{60.0f,-80.0f},{0.0f,-150.0f},{-50.0f,-150.0f} }, 15);
	path.AddCurve(BezierCurve{ {-50.0f,-150.0f},{-100.0f,-150.0f},{-125.0f,-200.0f},{-125.0f,-400.0f} }, 15);
	path.AddCurve(BezierCurve{ {-125.0f,-400.0f},{-125.0f,-500.0f},{-50.0f,-500.0f},{-50.0f,-400.0f} }, 15);

	int currentPath{ 0 };
	m_ZakoDivePath[currentPath++] = path.Sample();

	// odd x is -
	path = BezierPath{};
	path.AddCurve(BezierCurve{ {0.0f,0.0f},{0.0f,45.0f},{-60.0f,45.0f},{-60.0f,0.0f} }, 15);
	path.AddCurve(BezierCurve{ {-60.0f,0.0f},{60.0f,-80.0f},{0.0f,-150.0f},{50.0f,-150.0f} }, 15);
	path.AddCurve(BezierCurve{ {50.0f,-150.0f},{100.0f,-150.0f},{125.0f,-200.0f},{125.0f,-400.0f} }, 15);
	path.AddCurve(BezierCurve{ {125.0f,-400.0f},{125.0f,-500.0f},{50.0f,-500.0f},{50.0f,-400.0f} }, 15);

	m_ZakoDivePath[currentPath] = path.Sample();

#ifdef _DEBUG
	m_TrailPath = SLD::GameObject::Create(world);
	LineComponent* line = m_TrailPath->AddComponent<LineComponent>({});
	line->points.insert(line->points.end(), m_ZakoDivePath[0].begin(), m_ZakoDivePath[0].end());
#endif
}

void ZakoDiveSystem::OnUpdate(SLD::GameObjectId, float dt, SLD::TransformComponent* transform,
	SLD::Box2DComponent* box2d, SpeedComponent* speed, DiveComponent* diveIn, EnemyTag* tag, ShootableComponent*, FormationComponent* formation)
{
	using namespace SLD;
	using namespace rtm;

	if (tag->type == EnemyType::Zako)
	{
		if (tag->state == EnemyStateNums::Dive)
		{
			// if row is odd then do zako right
			// if row is even then do zako left
			int currentPath{ formation->row % 2 == 0 ? 0 : 1 };
			diveIn;
			transform;
			formation;
			//box2d->SetVelocity({  dt * speed->value * 1000.f,dt * speed->value * 1000.f });

			const auto& worldPos{ transform->GetWorldPos() };
			const auto initialStart{ vector_load2(&diveIn->initialPosition.x) };
			const auto currentP{ vector_load2(&worldPos.x) };

			if (size_t(diveIn->currentWayPoint) < m_ZakoDivePath[currentPath].size())
			{
				auto& path{ m_ZakoDivePath[currentPath] };

				auto targetP{ vector_load2(&path[diveIn->currentWayPoint].x) };

#ifdef _DEBUG
				LineComponent* line = m_TrailPath->GetComponent<LineComponent>();
				line->points.clear();

				for (auto& zakoDive : m_ZakoDivePath[currentPath])
				{
					auto trail{ vector_load2(&zakoDive) };
					trail = vector_add(initialStart, trail);
					float2f two{};
					vector_store2(trail, &two);
					line->points.emplace_back(two);
				}

#endif


				// offset from start in the formation
				targetP = vector_add(targetP, initialStart);

				auto vecToP{ vector_sub(targetP,currentP) };
				vecToP = vector_normalize3(vecToP);
				float2f slideVec{};
				vector_store2(vecToP, &slideVec);

				box2d->SetVelocity({ slideVec.x * dt * speed->value,slideVec.y * dt * speed->value });

				if (vector_length_squared(vector_sub(targetP, currentP)) <= 5.0f)
				{
					diveIn->currentWayPoint++;
				}

			}
			else
			{
				auto vecToP{ vector_sub(initialStart,currentP) };
				vecToP = vector_normalize3(vecToP);
				float2f slideVec{};
				vector_store2(vecToP, &slideVec);
				box2d->SetVelocity({ slideVec.x * dt * speed->value,slideVec.y * dt * speed->value });

				if (vector_length_squared(vector_sub(initialStart, currentP)) <= 1.0f)
				{
					box2d->SetVelocity({ 0.0f,0.0f });
					diveIn->currentWayPoint = 0;
					tag->state = EnemyStateNums::Formation;
				}

			}
		}
	}
}
