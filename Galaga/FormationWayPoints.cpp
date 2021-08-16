#include "FormationWayPoints.h"

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
