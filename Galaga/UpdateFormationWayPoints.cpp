#include "UpdateFormationWayPoints.h"
#include "EnemyPath.h"

//UpdateFormationWayPoints::UpdateFormationWayPoints(SLD::WorldEntity& world, const rtm::float2f& leftRight, float hopTime)
//	: SystemTemplate(world)
//	, m_LeftRightLimits(leftRight)
//	, m_HopInterval(hopTime)
//	, m_Timer()
//	, m_Step(15.0f)
//	, m_HopCount()
//	, m_HopMax(3)
//	, m_Swap(1)
//{
//}

UpdateFormationWayPoints::UpdateFormationWayPoints(SLD::WorldEntity& world, float hopTime)
	: SystemTemplate(world)
	, m_LeftRightLimits()
	, m_HopInterval(hopTime)
	, m_Timer()
	, m_Step(15.0f)
	, m_SpreadStep(10.0f)
	, m_SpreadMaxCount(4)
	, m_SpreadCount()
	, m_HopCount()
	, m_HopMax(3)
	, m_Swap(1)
{
}

void UpdateFormationWayPoints::OnUpdate(SLD::GameObjectId, float dt, FormationWayPoints* item)
{
	if (m_Timer < m_HopInterval)
		m_Timer += dt;
	else
	{
		m_Timer -= m_HopInterval;

		if (item->IsLocked())
		{
			// Do formation breathing
			BreathingWayPoints(item);
			return;
		}

		UpdateWayPoints(item->GetWayPoints());

		//auto finishedTask = m_World.PushAsyncTask([this, item]()
		//	{
		//		UpdateWayPoints(item->GetWayPoints());
		//	});

		//item->SetLinkTask(std::move(finishedTask));
	}

}

void UpdateFormationWayPoints::BreathingWayPoints(FormationWayPoints* points)
{
	// reset
	if (m_HopCount != 0)
	{
		UpdateWayPoints(points->GetWayPoints());
	}
	else
	{
		float left{ -1.0f };
		float right{ 1.0f };
		
		if (m_SpreadCount == m_SpreadMaxCount)
		{
			//left = 1.0f;
			//right = -1.0f;
			m_SpreadSwap *= -1;
			m_SpreadCount = 0;
		}

		const int maxRow{ 5 };

		// the first row doesn't move

		for (auto it = points->GetWayPoints().begin() + 1; it != points->GetWayPoints().end(); ++it)
		{
			float offset{ 1.0f / float(m_SpreadMaxCount) };
			for (size_t i = 0; i < it->size(); ++i)
			{
				auto& point{ (*it)[i] };

				if (i != 0 && i % 2 == 0)
				{
					//sideCnt++;
					offset += 1.0f / float(m_SpreadMaxCount);
				}
				
				if (i % 2 == 0) // odd goes left, even goes right
				{
					point.x += m_SpreadStep * right * offset * m_SpreadSwap;
				}
				else
					point.x += m_SpreadStep * left * offset * m_SpreadSwap;
			}
		}

		m_SpreadCount++;
	}
}

void UpdateFormationWayPoints::UpdateWayPoints(LineWayPoints& line)
{
	for (auto& lp : line)
	{
		for (auto& point : lp)
		{
			point.x += m_Step * float(m_Swap);
		}
	}

	if (m_HopCount == m_HopMax)
	{
		m_Swap *= -1;
		m_HopMax *= -1;
	}
	else
	{
		m_HopCount += 1 * m_Swap;
	}

}
