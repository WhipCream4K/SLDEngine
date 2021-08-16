#pragma once

#include "MyComponents.h"

struct SpawnData
{
	std::map<SpawnDirection, std::vector<EnemyType>> wave{};
	float interval{};
};

using StageSpawnData = std::vector<std::vector<SpawnData>>;