#include "GameModeSystem.h"

#include "GalagaScene.h"
#include "MyComponents.h"
#include "ResourceManager.h"
#include "GameStateComponent.h"
#include "IntervalState.h"
#include "FormationWayPoints.h"
#include "ReturnToMenu.h"
#include <Components/TransformComponent.h>
#include "EnemyPath.h"

GameModeSystem::GameModeSystem(SLDWorldEntity& world)
	: SystemTemplate(world)
	, m_OnReturnToMenu(std::make_shared<ReturnToMenu>())
{
	m_OnReturnToMenu->Bind([this](bool state) { OnReturnToMenu(state); });
	world.RegisterListener(m_OnReturnToMenu);
}

void GameModeSystem::OnUpdate(SLD::GameObjectId, float, GameModeComponent* gameMode)
{
	using namespace SLD;

	if (gameMode->isInActive)
		return;

	if (gameMode->shouldRestartGame)
	{
		gameMode->shouldRestartGame = false;
		SpawnMenu();
	}

	InputSettings& input{ m_World.GetWorldInputSetting() };
	if (input.GetInputState("Select") == InputEvent::IE_Pressed)
	{
		// Move Pointer accordingly
		m_CurrentSelectedPlayerStyle = (m_CurrentSelectedPlayerStyle + 1) % m_MaxPlayerCount;

		const auto& nextTransform{ m_World.GetComponent<TransformComponent>(
			m_Player1Text + m_CurrentSelectedPlayerStyle) };

		const auto& pointerTransform{ m_World.GetComponent<TransformComponent>(m_Pointer) };
		const auto& currentWorldPos{ pointerTransform->GetWorldPos() };

		pointerTransform->Translate(currentWorldPos.x, nextTransform->GetWorldPos().y - 50.0f, currentWorldPos.z);
	}
	if (input.GetInputState("Start") == InputEvent::IE_Pressed)
	{
		if (m_CurrentSelectedPlayerStyle == 0)
		{
			m_GameState1 = SpawnGameState(0);

			gameMode->currentPlayStyle = PlayStyle::Solo;
			gameMode->currentSpawnPlayer = 1;
		}
		else
		{
			m_GameState1 = SpawnGameState(0);
			m_GameState2 = SpawnGameState(1);

			gameMode->currentPlayStyle = PlayStyle::Coop;
			gameMode->currentSpawnPlayer = 2;
		}

		gameMode->isInActive = true;
		
		Clear();
	}
}

void GameModeSystem::OnReturnToMenu(bool)
{
	const auto gameMode = m_World.QueryGameObject<GameModeComponent>();
	SLD::GameObjectId currentGameMode{ gameMode[0] };
	GameModeComponent* gamemodeComp{ m_World.GetComponent<GameModeComponent>(currentGameMode) };
	if(gamemodeComp)
	{
		gamemodeComp->isInActive = false;
		gamemodeComp->shouldRestartGame = true;
		gamemodeComp->currentSpawnPlayer = 0;
	}

	m_World.DestroyGameObject(m_GameState1);
	m_World.DestroyGameObject(m_GameState2);
	
}

void GameModeSystem::SpawnMenu()
{
	using namespace SLD;

	const auto& resourceManger{ Instance<ResourceManager>() };

	sf::Font* font{ Instance<ResourceManager>()->Get<sf::Font>(GalagaScene::MainFont) };
	sf::Texture* mainSprite{ resourceManger->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };

	if (font && mainSprite)
	{
		const std::string playOnString{ "1 Player" };
		//const std::string playOnString2{ "2 Players" };
		const size_t fontSize{ 30 };

		const auto player1Object{ GameObject::Create(m_World) };
		m_Player1Text = player1Object->GetId();
		TextRenderComponent* test = player1Object->AddComponent<TextRenderComponent>({ font,playOnString,fontSize });
		test;

		const auto& player1Transform{ player1Object->GetComponent<TransformComponent>() };

		//const auto player2Object{ GameObject::Create(m_World) };
		//m_Player2Text = player2Object->GetId();
		//player2Object->AddComponent<TextRenderComponent>({ font,playOnString2,fontSize });

		//player2Object->GetComponent<TransformComponent>()->Translate(0.0f, -float(fontSize), 1.0f);

		const Vector4<uint32_t> rect{ 289,136,16,16 };

		const auto pointer{ GameObject::Create(m_World) };
		pointer->AddComponent<SpriteRenderComponent>({ *mainSprite,rect,{GalagaScene::GlobalScale} });
		m_Pointer = pointer->GetId();

		// set pointer to the first "1 Player"
		pointer->GetComponent<TransformComponent>()->Translate(-float(fontSize), player1Transform->GetWorldPos().y - float(fontSize), 1.0f);
	}
}

SLD::GameObjectId GameModeSystem::SpawnFormationTracker()
{
	using namespace SLD;

	const auto& enemyPath{ Instance<EnemyPath>() };
	const auto& tracker{ SLD::GameObject::Create(m_World) };
	tracker->AddComponent<FormationWayPoints>({enemyPath->GetFormationWayPoints()});
	enemyPath->SetFormationTracker(tracker->GetId());

	return tracker->GetId();
}

SLD::GameObjectId GameModeSystem::SpawnGameState(size_t playerIdx)
{
	auto out = SLD::GameObject::Create(m_World);
	GameStateComponent* stateComp = out->AddComponent<GameStateComponent>({40,3,3,playerIdx});

	if (playerIdx == 0)
		stateComp->isActive = true;

	stateComp->parent = out->GetId();
	stateComp->state = GameState::Interval;
	stateComp->state->Enter(m_World,stateComp);
	
	return out->GetId();
}

void GameModeSystem::Clear()
{
	m_World.DestroyGameObject(m_Player1Text);
	m_World.DestroyGameObject(m_Player2Text);
	m_World.DestroyGameObject(m_Pointer);
}
