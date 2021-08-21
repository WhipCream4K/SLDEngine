#include "GameStateSystem.h"
#include "PlaySystem.h"
#include "GalagaScene.h"
#include "EnemyStateSystem.h"
#include <Helpers/utils.h>

#include "Player.h"

GameStateSystem::GameStateSystem(SLDWorldEntity& world)
	: SystemTemplate(world)
{
	//OnPlayerDead->Bind([this](bool holder) { PlayerDead(holder); });
}

GameStateSystem::GameStateSystem(SLD::WorldEntity& world, SLD::GameObjectId gameStateObject)
	: SystemTemplate(world)
	, m_GameStateObject(gameStateObject)
{
	OnPlayerDead->Bind([this](SLD::GameObjectId playerId, bool holder) { PlayerDead(playerId, holder); });
}

void GameStateSystem::OnUpdate(SLD::GameObjectId, float dt, GameStateComponent* game, InputListener*)
{
	const auto& oldState{ game->state };
	const auto newState{ game->state->HandleInput(m_World,game) };

	if (newState)
	{
		oldState->Exit(m_World, game);
		newState->Enter(m_World, game);

		game->state = newState;
	}

	game->state->Update(m_World, dt, game);
}

void GameStateSystem::PlayerDead(SLD::GameObjectId, bool)
{
	if(GameStateComponent * gameState{ m_World.GetComponent<GameStateComponent>(m_GameStateObject) }; 
		gameState)
	{
		gameState->playerDead = true;
	}
}

void GameStateSystem::HandleMenuState(GameStateComponent* game)
{
	// Spawn UI, background
	using namespace SLD;
	game;
	//InputSettings& input{ m_World.GetWorldInputSetting() };
	//if (input.GetInputState("Enter") == InputEvent::IE_Pressed)
	//{
	//	game->state = GameStateNum::Play;
	//}
}

void GameStateSystem::HandlePlayState(GameStateComponent*)
{
	// Spawn player, enemy and etc...
	using namespace SLD;

	//bool& isSpawn{ m_StateBits[size_t(GameStateNum::Play)] };
	//if (!isSpawn)
	//{
	//	isSpawn = true;

	//	auto& systems{ m_SystemStateArray[size_t(GameStateNum::Play)] };

	//	const auto& enemyPath{ Instance<EnemyPath>() };
	//	const auto formationTracker{ SLD::GameObject::Create(m_World) };
	//	enemyPath->SetFormationTracker(formationTracker->GetId());
	//	formationTracker->AddComponent<FormationWayPoints>({ Instance<EnemyPath>()->GetFormationWayPoints() });

	//	systems.push_back(m_World.AddSystem<PlayerInputSystem>({ m_World }));
	//	systems.push_back(m_World.AddSystem<OnBulletContact>({ m_World }));

	//	const rtm::float4f playArea{ 0,0,550.0f,720.0f };
	//	const float hopInterval{ 0.5f };

	//	systems.push_back(m_World.AddSystem<LimitPlayerToPlayArea>({ m_World, playArea }));
	//	systems.push_back(m_World.AddSystem<UpdateProjectile>({ m_World,1000.0f }));
	//	systems.push_back(m_World.AddSystem<UpdateParticle>({ m_World }));
	//	systems.push_back(m_World.AddSystem<ReloadShooter>({ m_World }));
	//	systems.push_back(m_World.AddSystem<EnemySpawnerSystem>({ m_World }));
	//	systems.push_back(m_World.AddSystem<EnemyStateSystem>({ m_World,formationTracker->GetId() }));


	//	systems.push_back(m_World.AddSystem<UpdateFormationWayPoints>({ m_World,hopInterval }));


	//	const rtm::float3f playerSpawnPoint{ 0.0f,-260.0f,float(size_t(GalagaScene::Layer::Player)) };
	//	for (size_t i = 0; i < m_PlayerCount; ++i)
	//	{
	//		InstantiatePrefab<Player>(m_World, { i }, { playerSpawnPoint });
	//	}

	//}
}
