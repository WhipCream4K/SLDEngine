#include "PlayState.h"
#include "GalagaScene.h"
#include "GameStateComponent.h"
#include "PlaySystem.h"
#include "MyComponents.h"
#include "ResourceManager.h"
#include "SpawnStages.h"
#include "EnemyPath.h"
#include "EnemyManager.h"
#include "IntervalState.h"
#include "OnFormationLocked.h"
#include <Components/TransformComponent.h>

#include "EnemyStateSystem.h"

SharedPtr<GameState> PlayState::HandleInput(SLDWorldEntity& , GameStateComponent* game)
{
	if (game->currentEnemyCount <= 0)
	{
		game->currentStageCnt++;
		return Interval;
	}

	if(game->currentStageCnt >= game->maxStage)
	{
		m_ShouldRestart = true;
		// TODO: return end screen
		return {};
	}

	return {};
}

void PlayState::Update(SLDWorldEntity& world, float dt, GameStateComponent* game)
{
	SpawnEnemy(world, dt, game->currentStageCnt);
	SignalEnemyLockedFormation(world);
}

void PlayState::Enter(SLDWorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;
	
	m_pSpawnPaths = &Instance<EnemyPath>()->GetAllSpawnPaths();
	m_pSpawnData = &Instance<SpawnStages>()->GetSpawnStage();

	// Initialize system at the start
	if(game->currentStageCnt == 0)
	{
		auto& systems{ m_ExistSystem };
		const auto& enemyPath{ Instance<EnemyPath>() };
		const auto formationTracker{ SLD::GameObject::Create(world) };
		enemyPath->SetFormationTracker(formationTracker->GetId());
		formationTracker->AddComponent<FormationWayPoints>({ Instance<EnemyPath>()->GetFormationWayPoints() });
		m_FormationTracker = formationTracker->GetId();

		systems.push_back(world.AddSystem<PlayerInputSystem>({ world }));
		systems.push_back(world.AddSystem<OnBulletContact>({ world }));

		const rtm::float4f playArea{ 0,0,550.0f,720.0f };
		const float hopInterval{ 0.5f };

		systems.push_back(world.AddSystem<LimitPlayerToPlayArea>({ world, playArea }));
		systems.push_back(world.AddSystem<UpdateProjectile>({ world,1000.0f }));
		systems.push_back(world.AddSystem<UpdateParticle>({ world }));
		systems.push_back(world.AddSystem<ReloadShooter>({ world }));
		systems.push_back(world.AddSystem<EnemyStateSystem>({ world,formationTracker->GetId() }));


		systems.push_back(world.AddSystem<UpdateFormationWayPoints>({ world,hopInterval }));


		game->currentEnemyCount = 40;
	}

}

void PlayState::Exit(SLDWorldEntity& world, GameStateComponent* game)
{
	m_WaveTimer = 0.0f;
	m_SpawnTimer = 0.0f;
	m_BusCounter = 0;
	m_SectionCounter = 0;
	m_IsSpawning = false;

	if (m_ShouldRestart)
	{
		game->currentStageCnt = 0;
		
		m_ShouldRestart = false;
		for (auto& system : m_ExistSystem)
		{
			world.RemoveSystem(system.lock());
		}

		m_SpawnEnemies.clear();
	}
}

//void PlayState::DisplayInterval(SLDWorldEntity& world, float dt, int stageDisplay)
//{
//	using namespace SLD;
//
//	if (m_DisplayTimer < m_MaxDisplayTime)
//	{
//		m_DisplayTimer += dt;
//
//		TransformComponent* stageTransform{ world.GetComponent<TransformComponent>(m_StageText) };
//		TextRenderComponent* stageText{ world.GetComponent<TextRenderComponent>(m_StageText) };
//
//		if (m_DisplayTimer < m_MaxDisplayTime / 2.0f)
//		{
//			// Display stage
//			if (stageText)
//			{
//				const std::string stageCnt{ "Stage " + std::to_string(stageDisplay) };
//				stageText->text.setString(stageCnt);
//			}
//
//			stageTransform->Translate(0.0f, 100.0f, 1.0f);
//		}
//		else
//		{
//			// Hide old stage text
//			stageTransform->Translate(0.0f, -720.0f, 0.0f);
//			TransformComponent* readyTransform{ world.GetComponent<TransformComponent>(m_ReadyText) };
//			readyTransform->Translate(0.0f, 100.0f, 1.0f);
//		}
//	}
//	else
//	{
//		m_DisplayTimer -= m_MaxDisplayTime;
//
//		TransformComponent* readyTransform{ world.GetComponent<TransformComponent>(m_ReadyText) };
//		readyTransform->Translate(0.0f, -720.0f, 0.0f);
//
//		m_DisplayInterval = false;
//	}
//}

void PlayState::SpawnEnemy(SLD::WorldEntity& world, float dt, int stage)
{
	const auto& spawnData{ m_pSpawnData->at(stage) };

	if (m_SectionCounter < spawnData.size())
	{
		SpawnEnemySection(world,dt, spawnData[m_SectionCounter]);
	}
	else
	{
		// the whole wave has finished
		// trigger should not spawn
		m_SectionCounter = 0;
		// signal that formation should lock
	}
}

void PlayState::SpawnEnemySection(SLD::WorldEntity& world, float dt, const SpawnData& section)
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

				enemyManager->Spawn(world, { startPoint.x,startPoint.y,enemyLayer }, spawnEnemy, dir);

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

void PlayState::SignalEnemyLockedFormation(SLD::WorldEntity& world)
{
	bool flyIn{ false };
	for (const auto& enemy : m_SpawnEnemies)
	{
		EnemyTag* tag{ world.GetComponent<EnemyTag>(enemy) };
		if (tag->state == EnemyStateNums::FlyIn)
			flyIn = true;
	}

	if(flyIn)
	{
		FormationWayPoints* formation{ world.GetComponent<FormationWayPoints>(m_FormationTracker) };
		if(formation)
		{
			formation->SetLock(true);
		}
	}
}
