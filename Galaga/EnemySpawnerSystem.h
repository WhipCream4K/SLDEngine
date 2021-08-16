#pragma once

#include <SLDFramework.h>
#include <Components/TransformComponent.h>
#include <System/SystemTemplate.h>
#include "MyComponents.h"
#include "GameStateComponent.h"
#include "BezierPath.h"
#include "SpawnData.h"
#include "SpawnPaths.h"

class EnemySpawnerSystem : public SLD::SystemTemplate<SLD::TransformComponent, GameStateComponent>
{
public:

	EnemySpawnerSystem(SLDWorldEntity& world);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, GameStateComponent*) override;

	static constexpr float TrainInterval{ 0.3f };

private:

	void SpawnEnemySection(float dt, const SpawnData& section);

	const StageSpawnData* m_pSpawnData;
	const SpawnPaths* m_pSpawnPaths;

	size_t m_BusCounter;
	size_t m_SectionCounter{};
	int m_ButterFlyRow{};
	int m_ButterFlyCol{};
	int m_BeeRow{};
	int m_BeeCol{};
	int m_SpawnAmount{};
	float m_WaveTimer;
	float m_SpawnTimer;
	bool m_IsSpawning;
};

