#include "FormationWayPoints.h"
#include "EnemyPath.h"

FormationWayPoints::FormationWayPoints(const LineWayPoints& lines)
	: m_WayPoints(lines)
	, m_UpdateWayPointsFinished()
{
}

LineWayPoints& FormationWayPoints::GetWayPoints()
{
	return m_WayPoints;
}

void FormationWayPoints::SetLinkTask(std::future<void>&& future)
{
	m_UpdateWayPointsFinished = std::move(future);
}

void FormationWayPoints::SetLock(bool value)
{
	m_IsLocked = value;
}

bool FormationWayPoints::IsLocked() const
{
	return m_IsLocked;
}

void FormationWayPoints::Reset()
{
	m_WayPoints = SLD::Instance<EnemyPath>()->GetFormationWayPoints();
}
