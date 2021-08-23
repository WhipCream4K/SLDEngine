#include "GameStateSystem.h"
#include "PlaySystem.h"
#include "GalagaScene.h"
#include "EnemyStateSystem.h"
#include <Helpers/utils.h>
#include "OnPlayerDead.h"
#include "OnEnemyDead.h"
#include "GameStateManager.h"
#include "GameState.h"
#include "OnPlayerShoot.h"
#include "Player.h"

GameStateSystem::GameStateSystem(SLDWorldEntity& world)
	: SystemTemplate(world)
	, m_OnPlayerDead(std::make_shared<OnPlayerDead>())
	, m_OnEnemyDead(std::make_shared<OnEnemyDead>())
	, m_OnPlayerShoot(std::make_shared<OnPlayerShoot>())
{
	m_OnPlayerDead->Bind([this](SLD::GameObjectId id, bool holder) { PlayerDead(id, holder); });
	m_OnEnemyDead->Bind([this](SLD::GameObjectId id) {EnemyDead(id); });
	m_OnPlayerShoot->Bind([this](SLD::GameObjectId id) {PlayerShoot(id); });

	world.RegisterListener(m_OnPlayerDead);
	world.RegisterListener(m_OnEnemyDead);
	world.RegisterListener(m_OnPlayerShoot);
}

void GameStateSystem::OnUpdate(SLD::GameObjectId, float dt, GameStateComponent* game)
{
	if (!game->isActive)
		return;

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

void GameStateSystem::PlayerDead(SLD::GameObjectId playerId, bool)
{
	const auto activeGameState = m_World.QueryGameObject<GameStateComponent>();

	for (auto id : activeGameState)
	{
		if (GameStateComponent* gameState{ m_World.GetComponent<GameStateComponent>(id) };
			gameState)
		{
			// the current one
			if (gameState->isActive)
			{
				gameState->currentPlayerLives--;
				gameState->isPlayerDead = true;

				// Update player lives ( could seperate this too)
				{
					SpriteRenderComponent* playerLivesSprite{ m_World.GetComponent<SpriteRenderComponent>(gameState->attachedHUD.d_Lives) };
					if (playerLivesSprite)
					{
						playerLivesSprite->sprite.setTextureRect({ 0,0,16 * gameState->currentPlayerLives,16 });
					}
				}

				// Destroy?
				{
					m_World.DestroyGameObject(playerId);
				}
			}
		}
	}
}

void GameStateSystem::EnemyDead(SLD::GameObjectId enemyId)
{
	const auto activeGameState = m_World.QueryGameObject<GameStateComponent>();

	for (auto id : activeGameState)
	{
		if (GameStateComponent* gameState{ m_World.GetComponent<GameStateComponent>(id) };
			gameState)
		{
			// the current one
			if (gameState->isActive)
			{
				gameState->activeEnemies.erase(
					std::find(
						gameState->activeEnemies.begin(),
						gameState->activeEnemies.end(),
						enemyId));

				gameState->currentEnemyDead++;
				
				EnemyTag* tag{ m_World.GetComponent<EnemyTag>(enemyId) };
				if (tag)
				{
					int earnedScore{};
					bool isDiving{ tag->state == EnemyStateNums::Dive ? true : false };
					switch (tag->type)
					{
					case EnemyType::Zako:		earnedScore = isDiving ? 100 : 50; break;
					case EnemyType::Goei:		earnedScore = isDiving ? 160 : 80; break;
					case EnemyType::Galagas:	earnedScore = isDiving ? 400 : 150; break;
					default: break;
					}

					gameState->currentScore += earnedScore;

					tag->state = EnemyStateNums::Died;
				}

				// Could have separate this
				{
					TextRenderComponent* scoreText{ m_World.GetComponent<TextRenderComponent>(gameState->attachedHUD.d_Score) };
					if (scoreText)
					{
						scoreText->text = std::to_string(gameState->currentScore);
					}
				}
			}
		}
	}
}

void GameStateSystem::PlayerShoot(SLD::GameObjectId)
{
	const auto activeGameState = m_World.QueryGameObject<GameStateComponent>();

	for (auto id : activeGameState)
	{
		if (GameStateComponent* gameState{ m_World.GetComponent<GameStateComponent>(id) };
			gameState)
		{
			// the current one
			if (gameState->isActive)
			{
				gameState->shotCount++;
			}
		}
	}
}
