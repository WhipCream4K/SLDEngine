#include "EnemySpawnerSystem.h"
#include "Zako.h"
#include "Goei.h"
#include <Helpers/utils.h>
#include "SpawnStages.h"
#include "GalagaScene.h"
#include "EnemyManager.h"
#include "SpawnStages.h"
#include "EnemyPath.h"

EnemySpawnerSystem::EnemySpawnerSystem(SLDWorldEntity& world)
	: SystemTemplate(world)
	, m_BusCounter()
	, m_ButterFlyRow()
	, m_ButterFlyCol()
	, m_BeeRow()
	, m_BeeCol()
	, m_WaveTimer()
	, m_SpawnTimer()
	, m_IsSpawning()
{
	// Query all path data here
	using namespace SLD;
	m_pSpawnData = &Instance<SpawnStages>()->GetSpawnStage();
	m_pSpawnPaths = &Instance<EnemyPath>()->GetAllSpawnPaths();
}

void EnemySpawnerSystem::OnUpdate(SLD::GameObjectId, float dt, SLD::TransformComponent*,
	GameStateComponent* gameState)
{
	using namespace SLD;
	constexpr float enemyLayer{ float(GalagaScene::Layer::Enemy) };

	// Spawn enemy at somewhat interval

	if (gameState->currentState == GameState::Play)
	{
		if (gameState->shouldSpawn)
		{
			if (gameState->playStage < m_pSpawnData->size())
			{
				const auto& spawnData{ m_pSpawnData->at(gameState->playStage) };

				if (m_SectionCounter < spawnData.size())
				{
					SpawnEnemySection(dt, spawnData[m_SectionCounter]);
				}
				else
				{
					// the whole wave has finished
					// trigger should not spawn
					gameState->shouldSpawn = false;
					m_SectionCounter = 0;
				}
			}
		}
	}
}

void EnemySpawnerSystem::SpawnEnemySection(float dt, const SpawnData& section)
{
	using namespace SLD;
	constexpr float enemyLayer{ float(GalagaScene::Layer::Enemy) };

	if (m_IsSpawning)
	{
		if (m_SpawnTimer < TrainInterval)
			m_SpawnTimer += dt;
		else
		{
			m_SpawnTimer -= TrainInterval;

			const auto& enemyManager{ Instance<EnemyManager>() };

			for (const auto& [dir, enemyTypes] : section.wave)
			{
				if (m_BusCounter >= enemyTypes.size())
				{
					// This section has ended
					// go to next section
					m_BusCounter = 0;
					m_SectionCounter++;
					m_IsSpawning = false;
					
					return;
				}

				const auto& spawnEnemy{ enemyTypes[m_BusCounter] };
				const auto& startPoint{ m_pSpawnPaths->at(size_t(dir))[0] };

				enemyManager->Spawn(m_World,{ startPoint.x,startPoint.y,enemyLayer }, spawnEnemy, dir);

			}
			
			m_BusCounter++;
		}


	}
	else
	{
		if (m_WaveTimer < section.interval)
			m_WaveTimer += dt;
		else
		{
			m_WaveTimer -= section.interval;
			m_IsSpawning = true;
		}
	}
}
