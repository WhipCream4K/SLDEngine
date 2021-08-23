#pragma once

#include <SLDFramework.h>
#include <Core/Singleton.h>
#include "MyComponents.h"
#include "BezierPath.h"
#include "SpawnPaths.h"

class EnemyPath : public SLD::Singleton<EnemyPath>
{
public:

	using LineWayPoints = std::vector<std::vector<rtm::float2f>>;
	
	friend class SLD::Singleton<EnemyPath>;
	
	EnemyPath();

	void Initialize();
	
	[[nodiscard]] const std::vector<rtm::float2f>& GetPath(SpawnDirection dir);
	[[nodiscard]] const SpawnPaths& GetAllSpawnPaths() const;
	const LineWayPoints& GetFormationWayPoints() const;
	void SetFormationTracker(SLD::GameObjectId id);
	void TrackerPathReset(SLD::WorldEntity& world);
	void Destroy(SLD::WorldEntity& world);
	[[nodiscard]] SLD::GameObjectId GetFormationTracker() const;

private:

	SpawnPaths m_Paths;
	LineWayPoints m_LineFormationWayPoints;
	SLD::GameObjectId m_FormationHolder;
	std::future<void> m_UpdateWayPointsFinished;
	size_t m_HopStep;
};

