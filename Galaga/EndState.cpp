#include "EndState.h"
#include "MyComponents.h"
#include "ResourceManager.h"
#include "GalagaScene.h"
#include "GameStateComponent.h"
#include "ShootableComponent.h"
#include "ReturnToMenu.h"
#include <Components/TransformComponent.h>

SharedPtr<GameState> EndState::HandleInput(SLDWorldEntity& world, GameStateComponent*)
{
	if (m_FinishedDisplay)
	{
		// Call game end
		world.BroadCast<ReturnToMenu>(true);
		return Null;
	}

	return {};
}

void EndState::Update(SLDWorldEntity&, float dt, GameStateComponent*)
{
	if (m_DisplayTimer < m_MaxDisplayTime)
		m_DisplayTimer += dt;
	else
		m_FinishedDisplay = true;
}

void EndState::Enter(SLDWorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;

	const size_t fontSize{ 25 };

	sf::Font* mainFont{ Instance<ResourceManager>()->Get<sf::Font>(GalagaScene::MainFont) };

	if (mainFont)
	{
		const float offset{ 5.0f };
		{
			const auto result{ GameObject::Create(world) };
			TextRenderComponent* text = result->AddComponent<TextRenderComponent>({ mainFont,"--RESULTS--",fontSize });
			text->color = sf::Color{255,0,0};

			TransformComponent* transform{ result->GetComponent<TransformComponent>() };
			transform->Translate(-float(fontSize) * offset, fontSize * 2, 1.0f);

			m_ResultText = result->GetId();
		}

		int shots{};
		// ShotsFired
		{
			const auto shotsFired{ GameObject::Create(world) };
			TextRenderComponent* text = shotsFired->AddComponent<TextRenderComponent>({ mainFont,"SHOTSFIRED:\t" + std::to_string(game->shotCount),fontSize });
			text->color = sf::Color{255,255,0};
			shots = game->shotCount;

			TransformComponent* transform{ shotsFired->GetComponent<TransformComponent>() };
			transform->Translate(-float(fontSize) * offset, fontSize, 1.0f);

			m_ShotsFiredText = shotsFired->GetId();
		}

		// Number of Hits
		{
			const auto numberOfHits{ GameObject::Create(world) };
			TextRenderComponent* text = numberOfHits->AddComponent<TextRenderComponent>({ mainFont,"NUMBER OF HITS:\t" +
				std::to_string(game->currentEnemyDead),fontSize });

			text->color = sf::Color{ 255,255,0 };

			TransformComponent* transform{ numberOfHits->GetComponent<TransformComponent>() };
			transform->Translate(-float(fontSize) * offset, -float(fontSize), 1.0f);

			m_NumberOfHitsText = numberOfHits->GetId();
		}

		// Hit of miss ratio
		{
			const int ratio{ shots > 0 ? game->currentEnemyDead / shots * 100 : 0 };
			//if(shots == 0)
			const auto hitOrMiss{ GameObject::Create(world) };
			hitOrMiss->AddComponent<TextRenderComponent>(
				{ mainFont,"HIT MISSRATIO:\t" + std::to_string(ratio),fontSize });

			TransformComponent* transform{ hitOrMiss->GetComponent<TransformComponent>() };
			transform->Translate(-float(fontSize) * offset, -float(fontSize) * 2.0f, 1.0f);

			m_HitMissRatioText = hitOrMiss->GetId();
		}
	}

	world.DestroyGameObject(game->playerId);
}

void EndState::Exit(SLDWorldEntity& world, GameStateComponent* game)
{
	// Clean gamestate
	m_FinishedDisplay = false;
	m_DisplayTimer = 0.0f;

	// Destroy HUD
	world.DestroyGameObject(game->attachedHUD.d_Score);
	world.DestroyGameObject(game->attachedHUD.d_Stage);
	world.DestroyGameObject(game->attachedHUD.d_Lives);
	world.DestroyGameObject(game->attachedHUD.s_1Up);

	world.DestroyGameObject(m_HitMissRatioText);
	world.DestroyGameObject(m_ShotsFiredText);
	world.DestroyGameObject(m_ResultText);
	world.DestroyGameObject(m_NumberOfHitsText);
}
