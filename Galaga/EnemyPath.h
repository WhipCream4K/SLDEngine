#pragma once

#include <SLDFramework.h>
#include <Core/Singleton.h>
#include "MyComponents.h"
#include "BezierPath.h"
#include "SpawnPaths.h"

class EnemyPath : public SLD::Singleton<EnemyPath>
{
public:

	using FormationWayPoints = std::vector<std::vector<rtm::float2f>>;
	
	friend class SLD::Singleton<EnemyPath>;
	
	EnemyPath();

	void Initialize();
	
	[[nodiscard]] const std::vector<rtm::float2f>& GetPath(SpawnDirection dir);
	[[nodiscard]] const SpawnPaths& GetAllSpawnPaths() const;
	const FormationWayPoints& GetFormationWayPoints() const;

private:

	
	SpawnPaths m_Paths;
	FormationWayPoints m_LineFormationWayPoints;
	std::future<void> m_UpdateWayPointsFinished;
	size_t m_HopStep;
};

