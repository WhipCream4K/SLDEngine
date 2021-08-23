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
#include "EndState.h"
#include "EnemyStateSystem.h"
#include "Player.h"
#include "OnPlayerDead.h"
#include <Helpers/utils.h>

SharedPtr<GameState> PlayState::HandleInput(SLDWorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;

	InputSettings& input{ world.GetWorldInputSetting() };

	// For skipping if needed
	if (input.GetInputState("Enter") == InputEvent::IE_Pressed)
	{
		game->currentStageCnt++;

		if (game->currentStageCnt >= game->maxStage)
			return End;

		return Interval;
	}

	
	if (game->currentPlayerLives <= 0)
		return End;

	if(game->isInMidStage)
	{
		if (game->activeEnemies.empty())
			return Interval;
	}
	
	
	return {};
}

void PlayState::Update(SLDWorldEntity& world, float dt, GameStateComponent* game)
{
	if (!game->isInMidStage)
	{
		if (game->activeEnemies.size() < size_t(game->maxEnemyCount))
		{
			SpawnEnemy(world, dt, game->currentStageCnt, game);
		}
		else
		{
			game->isInMidStage = true;
			m_SectionCounter = 0;
		}
	}
	else
	{
		// Respawn maybe

		if (!game->isFormationLocked)
			SignalEnemyLockedFormation(world, game);

		if (!game->isPlayerDead)
			RandomizeDive(world, game);
		else
			SpawnPlayer(world, game);
	}
}

void PlayState::Enter(SLDWorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;

	m_pSpawnPaths = &Instance<EnemyPath>()->GetAllSpawnPaths();
	m_pSpawnData = &Instance<SpawnStages>()->GetSpawnStage();

	game->isStageStart = true;

	if (!game->isPlayerSet)
	{
		game->isPlayerSet = true;
		
		// Spawn Player Once
		const rtm::float3f playerSpawnPoint{ 0.0f,-260.0f,float(size_t(GalagaScene::Layer::Player)) };
		const auto id = InstantiatePrefab<Player>(world, { game->playerIndex }, playerSpawnPoint);
		game->playerId = id;
	}
}

void PlayState::Exit(SLDWorldEntity& world, GameStateComponent* game)
{
	m_WaveTimer = 0.0f;
	m_SpawnTimer = 0.0f;
	m_BusCounter = 0;
	m_SectionCounter = 0;
	m_IsSpawning = false;

	game->isFormationLocked = false;
	game->isInMidStage = false;
	game->isStageStart = false;

	Clear(world, game);

	game->activeEnemies.clear();

	FormationWayPoints* formation = world.GetComponent<FormationWayPoints>(SLD::Instance<EnemyPath>()->GetFormationTracker());
	if (formation)
	{
		formation->SetLock(false);
		formation->Reset();
	}
}

void PlayState::SpawnEnemy(SLD::WorldEntity& world, float dt, int stage, GameStateComponent* game)
{
	if (stage < game->maxStage)
	{
		const auto& spawnData{ m_pSpawnData->at(stage) };

		if (m_SectionCounter < spawnData.size())
		{
			SpawnEnemySection(world, dt, spawnData[m_SectionCounter], game);
		}
		else
		{
			// the whole wave has finished
			// trigger should not spawn
			m_SectionCounter = 0;
			// signal that formation should lock
			game->isInMidStage = true;
		}
	}
}

void PlayState::SpawnEnemySection(SLD::WorldEntity& world, float dt, const SpawnData& section, GameStateComponent* game)
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

				game->activeEnemies.emplace_back(enemyManager->Spawn(world, { startPoint.x,startPoint.y,enemyLayer }, spawnEnemy, dir,EnemyStateNums::FlyIn));

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

void PlayState::SignalEnemyLockedFormation(SLD::WorldEntity& world, GameStateComponent* game)
{
	bool alreadyInformation{ true };
	for (const auto& enemy : game->activeEnemies)
	{
		FormationComponent* formationComp{ world.GetComponent<FormationComponent>(enemy) };
		if (!formationComp->alreadyInside)
			alreadyInformation = false;
	}

	if (alreadyInformation)
	{
		FormationWayPoints* formation{ world.GetComponent<FormationWayPoints>(SLD::Instance<EnemyPath>()->GetFormationTracker()) };
		if (formation)
		{
			formation->SetLock(true);
			game->isFormationLocked = true;
		}
	}
}

void PlayState::RandomizeDive(SLD::WorldEntity&, GameStateComponent*)
{

}

void PlayState::SpawnPlayer(SLD::WorldEntity& world, GameStateComponent* game)
{
	const auto parent{ game->parent };

	if (!m_WaitForSignal.valid())
	{
		m_WaitForSignal = world.PushAsyncTask([&world, parent]()
			{
				while (true)
				{
					GameStateComponent* gameState{ world.GetComponent<GameStateComponent>(parent) };
					if (gameState)
					{
						bool isAllInFormation{ false };
						for (auto id : gameState->activeEnemies)
						{
							EnemyTag* tag{ world.GetComponent<EnemyTag>(id) };
							if (tag)
							{
								if (tag->state == EnemyStateNums::Formation)
									isAllInFormation = true;
							}
						}

						if (isAllInFormation)
							break;
					}

				}
			});
	}

	if (m_WaitForSignal.valid())
	{
		if (m_WaitForSignal.wait_for(std::chrono::seconds{ 0 }) == std::future_status::ready)
		{
			m_WaitForSignal.get();

			const rtm::float3f playerSpawnPoint{ 0.0f,-260.0f,float(size_t(GalagaScene::Layer::Player)) };
			const auto id = SLD::InstantiatePrefab<Player>(world, { game->playerIndex }, playerSpawnPoint);
			game->playerId = id;
			game->isPlayerDead = false;
		}
	}
}

void PlayState::Clear(SLD::WorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;

	for (auto& id : game->activeEnemies)
	{
		EnemyTag* tag{ world.GetComponent<EnemyTag>(id) };
		if (tag)
			tag->state = EnemyStateNums::Died;
		
		Instance<EnemyManager>()->Hide(world, id);
	}

	Instance<EnemyManager>()->Reset();
}
