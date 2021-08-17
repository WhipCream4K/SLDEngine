#include "GameStateSystem.h"
#include "PlaySystem.h"
#include "GalagaScene.h"
#include "EnemyStateSystem.h"
#include <Helpers/utils.h>

#include "Player.h"

GameStateSystem::GameStateSystem(SLDWorldEntity& world)
	: SystemTemplate(world)
	, m_StateBits()
	, m_SystemStateArray()
	, m_PlayerCount(1)
{
}

void GameStateSystem::OnUpdate(SLD::GameObjectId , float , GameStateComponent* game, InputListener*)
{
	switch (game->currentState)
	{
	case GameState::Menu:

		HandleMenuState(game);

		break;
	case GameState::Play:

		HandlePlayState(game);

		break;
	case GameState::End: break;
	case GameState::Count: break;
	default: break;
	}
}
void GameStateSystem::HandleMenuState(GameStateComponent* game)
{
	// Spawn UI, background
	using namespace SLD;

	InputSettings& input{ m_World.GetWorldInputSetting() };

	if (input.GetInputState("Enter") == InputEvent::IE_Pressed)
	{
		game->currentState = GameState::Play;
	}
}

void GameStateSystem::HandlePlayState(GameStateComponent*)
{
	// Spawn player, enemy and etc...
	using namespace SLD;

	bool& isSpawn{ m_StateBits[size_t(GameState::Play)] };
	if (!isSpawn)
	{
		isSpawn = true;

		auto& systems{ m_SystemStateArray[size_t(GameState::Play)] };

		const auto formationTracker{ SLD::GameObject::Create(m_World) };
		formationTracker->AddComponent<FormationWayPoints>({ Instance<EnemyPath>()->GetFormationWayPoints() });
		
		systems.push_back(m_World.AddSystem<PlayerInputSystem>({ m_World }));
		systems.push_back(m_World.AddSystem<OnBulletContact>({ m_World }));

		const rtm::float4f playArea{ 0,0,550.0f,720.0f };
		const float hopInterval{0.5f};
		
		systems.push_back(m_World.AddSystem<LimitPlayerToPlayArea>({ m_World, playArea }));
		systems.push_back(m_World.AddSystem<UpdateProjectile>({ m_World,1000.0f }));
		systems.push_back(m_World.AddSystem<UpdateParticle>({ m_World }));
		systems.push_back(m_World.AddSystem<ReloadShooter>({ m_World }));
		systems.push_back(m_World.AddSystem<EnemySpawnerSystem>({ m_World }));
		systems.push_back(m_World.AddSystem<EnemyStateSystem>({ m_World,formationTracker->GetId() }));


		systems.push_back(m_World.AddSystem<UpdateFormationWayPoints>({ m_World,hopInterval }));
		
		
		const rtm::float3f playerSpawnPoint{ 0.0f,-260.0f,float(size_t(GalagaScene::Layer::Player)) };
		for (size_t i = 0; i < m_PlayerCount; ++i)
		{
			InstantiatePrefab<Player>(m_World, {i}, {playerSpawnPoint});
		}

	}
}
