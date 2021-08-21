#pragma once

#include <Components/BaseComponent.h>

using LineWayPoints = std::vector<std::vector<rtm::float2f>>;
class FormationWayPoints : public SLD::ComponentT<FormationWayPoints>
{
public:

	FormationWayPoints(const LineWayPoints& lines);
	LineWayPoints& GetWayPoints();
	void SetLinkTask(std::future<void>&& future);
	void SetLock(bool value);
	bool IsLocked() const;

	

private:

	LineWayPoints m_WayPoints;
	std::future<void> m_UpdateWayPointsFinished;
	bool m_IsLocked{};
};

