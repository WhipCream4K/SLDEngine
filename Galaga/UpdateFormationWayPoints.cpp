#include "UpdateFormationWayPoints.h"

UpdateFormationWayPoints::UpdateFormationWayPoints(SLD::WorldEntity& world, const rtm::float2f& leftRight, float hopTime)
	: SystemTemplate(world)
	, m_LeftRightLimits(leftRight)
	, m_HopInterval(hopTime)
	, m_Timer()
	, m_Step(15.0f)
	, m_HopCount()
	, m_HopMax(3)
	, m_Swap(1)
{
}

UpdateFormationWayPoints::UpdateFormationWayPoints(SLD::WorldEntity& world, float hopTime)
	: SystemTemplate(world)
	, m_LeftRightLimits()
	, m_HopInterval(hopTime)
	, m_Timer()
	, m_HopCount()
	, m_HopMax(3)
	, m_Swap(1)
	, m_Step(15.0f)
{
}

void UpdateFormationWayPoints::OnUpdate(SLD::GameObjectId, float dt, FormationWayPoints* item)
{
	if (m_Timer < m_HopInterval)
		m_Timer += dt;
	else
	{
		m_Timer -= m_HopInterval;

		UpdateWayPoints(item->GetWayPoints());

		//auto finishedTask = m_World.PushAsyncTask([this, item]()
		//	{
		//		UpdateWayPoints(item->GetWayPoints());
		//	});

		//item->SetLinkTask(std::move(finishedTask));
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
