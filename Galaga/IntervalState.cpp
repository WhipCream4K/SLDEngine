#include "IntervalState.h"
#include "PlayState.h"
#include "GalagaScene.h"
#include "ResourceManager.h"

#include <Components/TransformComponent.h>

#include "GameStateComponent.h"

SharedPtr<GameState> IntervalState::HandleInput(SLDWorldEntity&, GameStateComponent* game)
{
	if (m_FinishedDisplay)
		return std::make_shared<PlayState>();
	
	game;
	//if(game->playStyle == PlayStyle::Coop)
	//{
	//	game->saveState = std::make_shared<PlayState>();
	//}

	return {};
}

void IntervalState::Update(SLDWorldEntity& world, float dt, GameStateComponent*)
{
	using namespace SLD;

	if (m_DisplayTimer < m_MaxDisplayTime)
	{
		m_DisplayTimer += dt;

		TransformComponent* stageTransform{ world.GetComponent<TransformComponent>(m_StageText) };
		//TextRenderComponent* stageText{ world.GetComponent<TextRenderComponent>(m_StageText) };

		if (m_DisplayTimer < m_MaxDisplayTime / 2.0f)
		{
			// Display stage
			stageTransform->Translate(0.0f, 100.0f, 1.0f);
		}
		else
		{
			// Hide old stage text
			stageTransform->Translate(0.0f, -720.0f, 0.0f);
			TransformComponent* readyTransform{ world.GetComponent<TransformComponent>(m_ReadyText) };
			readyTransform->Translate(0.0f, 100.0f, 1.0f);
		}
	}
	else
	{
		
		TransformComponent* readyTransform{ world.GetComponent<TransformComponent>(m_ReadyText) };
		readyTransform->Translate(0.0f, -720.0f, 0.0f);
		m_FinishedDisplay = true;
	}
}

void IntervalState::Enter(SLDWorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;
	
	// Initialize stage text
	sf::Font* font{ Instance<ResourceManager>()->Get<sf::Font>(GalagaScene::MainFont) };

	if (font)
	{
		const size_t fontSize{ 30 };
		
		const auto textObject{ GameObject::Create(world) };

		const std::string stageNum{ "Stage " + std::to_string(game->currentStageCnt + 1) };
		
		TextRenderComponent* textComp = textObject->AddComponent<TextRenderComponent>({ font,stageNum,fontSize });
		
		textComp->color = sf::Color{ 31,114,240 };

		m_StageText = textObject->GetId();

		const auto textObject2{ GameObject::Create(world) };
		textComp = textObject2->AddComponent<TextRenderComponent>({ font,"Ready",fontSize });

		textComp->color = sf::Color{ 189,38,38 };
		
		m_ReadyText = textObject2->GetId();

		// HIDE
		textObject->GetComponent<TransformComponent>()->Translate(0.0f, -720.0f, 1.0f);
		textObject2->GetComponent<TransformComponent>()->Translate(0.0f, -720.0f, 1.0f);
	}

	if(!game->isHUDInitialized)
	{
		game->isHUDInitialized = true;
		InitializedHUD(world, game);
	}
}

void IntervalState::Exit(SLDWorldEntity& world, GameStateComponent* game)
{
	m_DisplayTimer = 0.0f;
	m_FinishedDisplay = false;

	world.DestroyGameObject(m_StageText);
	world.DestroyGameObject(m_ReadyText);

	// Update Stage
	SpriteRenderComponent* sprite{ world.GetComponent<SpriteRenderComponent>(game->attachedHUD.d_Stage) };
	if(sprite)
	{
		sprite->sprite.setTextureRect({ 0,0, 16 * (game->currentStageCnt + 1),16 });
	}
}

void IntervalState::InitializedHUD(SLD::WorldEntity& world, GameStateComponent* game)
{
	using namespace SLD;
	const size_t fontSize{30};

	sf::Font* font{ SLD::Instance<ResourceManager>()->Get<sf::Font>(GalagaScene::MainFont) };
	sf::Texture* mainTexture{ Instance<ResourceManager>()->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };
	if(font && mainTexture)
	{
		// 1 Up
		{
			const auto s_1Up{ GameObject::Create(world) };
			TextRenderComponent* text = s_1Up->AddComponent<TextRenderComponent>({ font,"1UP",fontSize });
			text->color = sf::Color{ 255,0,0 };
		
			s_1Up->GetComponent<TransformComponent>()->Translate(330.0f,100.0f,0.0f);
			game->attachedHUD.s_1Up = s_1Up->GetId();
		}

		// Score
		{
			const float offset{10.0f};
			const auto d_Score{ GameObject::Create(world) };
			d_Score->AddComponent<TextRenderComponent>({ font,"0",fontSize });
			d_Score->GetComponent<TransformComponent>()->Translate(360.0f, 70.0f - offset, 0.0f);
			game->attachedHUD.d_Score = d_Score->GetId();
		}

		// Stage
		{
			sf::Texture* stage{ Instance<ResourceManager>()->Get<sf::Texture>("Stage") };
			if(stage)
			{
				const auto d_Stage{ GameObject::Create(world) };
				d_Stage->AddComponent<SpriteRenderComponent>({ *stage,{0,0,0,0},GalagaScene::GlobalScale });
				d_Stage->GetComponent<TransformComponent>()->Translate(400.0f, -200.0f, 0.0f);
				game->attachedHUD.d_Stage = d_Stage->GetId();
			}
		}

		// Lives
		{
			sf::Texture* player{ Instance<ResourceManager>()->Get<sf::Texture>("Player") };
			if (player)
			{
				const auto d_Lives{ GameObject::Create(world) };
				d_Lives->AddComponent<SpriteRenderComponent>({ *player,{0,0,uint32_t(16 * game->currentPlayerLives),16},GalagaScene::GlobalScale });
				d_Lives->GetComponent<TransformComponent>()->Translate(400.0f, -150.0f, 0.0f);
				game->attachedHUD.d_Lives = d_Lives->GetId();
			}
		}
	}
}
