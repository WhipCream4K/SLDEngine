#pragma once

#include "MyComponents.h"
#include <Core/Singleton.h>
#include "EnemyVariable.h"

using RowCol = std::pair<int, int>;

class EnemyManager : public SLD::Singleton<EnemyManager>
{
public:

	friend class Singleton<EnemyManager>;

	EnemyManager();
	
	SLD::GameObjectId Spawn(SLD::WorldEntity& world, const rtm::float3f& pos, EnemyType type, SpawnDirection dir);
	SLD::GameObjectId Spawn(SLD::WorldEntity& world, const rtm::float3f& pos, EnemyType type, SpawnDirection dir, EnemyStateNums state);
	void Hide(SLD::WorldEntity& world,SLD::GameObjectId id);
	void Reset();

private:

	void InitializeRowCol();
	
	std::map<EnemyType, std::vector<RowCol>> m_EnemyRowColFormation;
	std::map<EnemyType, std::vector<SharedPtr<SLD::GameObject>>> m_EnemyInstances;
	std::map<EnemyType, size_t> m_ActiveEnemyCounter;
	std::map<EnemyType, size_t> m_MaxEnemyCounter;
};

