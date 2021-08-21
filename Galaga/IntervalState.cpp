#include "IntervalState.h"
#include "PlayState.h"
#include "GalagaScene.h"
#include "ResourceManager.h"

#include <Components/TransformComponent.h>

#include "GameStateComponent.h"

SharedPtr<GameState> IntervalState::HandleInput(SLDWorldEntity&, GameStateComponent*)
{
	if (m_FinishedDisplay)
		return std::make_shared<PlayState>();

	return {};
}

void IntervalState::Update(SLDWorldEntity& world, float dt, GameStateComponent* game)
{
	using namespace SLD;

	if (m_DisplayTimer < m_MaxDisplayTime)
	{
		m_DisplayTimer += dt;

		TransformComponent* stageTransform{ world.GetComponent<TransformComponent>(m_StageText) };
		TextRenderComponent* stageText{ world.GetComponent<TextRenderComponent>(m_StageText) };

		if (m_DisplayTimer < m_MaxDisplayTime / 2.0f)
		{
			// Display stage
			if (stageText)
			{
				const std::string stageCnt{ "Stage " + std::to_string(game->currentStageCnt) };
				stageText->text = stageCnt;
			}

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

void IntervalState::Enter(SLDWorldEntity& world, GameStateComponent*)
{
	using namespace SLD;
	
	// Initialize stage text
	sf::Font* font{ Instance<ResourceManager>()->Get<sf::Font>(GalagaScene::MainFont) };

	if (font)
	{
		const auto textObject{ GameObject::Create(world) };
		TextRenderComponent* textComp = textObject->AddComponent<TextRenderComponent>({ font,"Stage",50 });

		//textComp->text.setFillColor(sf::Color{ 31,115,240 });
		textComp->color = sf::Color{ 31,114,240 };

		m_StageText = textObject->GetId();

		const auto textObject2{ GameObject::Create(world) };
		textComp = textObject2->AddComponent<TextRenderComponent>({ font,"Ready",50 });

		//textComp->text.setFillColor(sf::Color{ 189,38,38 });
		textComp->color = sf::Color{ 189,38,38 };

		m_ReadyText = textObject2->GetId();
	}
}

void IntervalState::Exit(SLDWorldEntity&, GameStateComponent*)
{
	m_DisplayTimer = 0.0f;
	m_FinishedDisplay = false;
}
