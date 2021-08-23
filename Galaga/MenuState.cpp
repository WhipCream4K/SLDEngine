#include "MenuState.h"
#include "MyComponents.h"
#include "ResourceManager.h"
#include "GalagaScene.h"
#include "PlayState.h"
#include <Components/TransformComponent.h>

#include "GameStateComponent.h"

MenuState::MenuState()
	: m_MaxPlayerCount(2)
{
}

SharedPtr<GameState> MenuState::HandleInput(SLDWorldEntity& world,GameStateComponent* )
{
	world;
	using namespace SLD;

	InputSettings& input{ world.GetWorldInputSetting() };
	
	if(input.GetInputState("UI") == InputEvent::IE_Pressed)
	{
		// Move Pointer accordingly
		m_CurrentSelectedPlayerStyle = (m_CurrentSelectedPlayerStyle + 1) % m_MaxPlayerCount;

		const auto& nextTransform{ world.GetComponent<TransformComponent>(
			m_Player1Text + m_CurrentSelectedPlayerStyle) };

		const auto& pointerTransform{ world.GetComponent<TransformComponent>(m_Pointer) };
		const auto& currentWorldPos{ pointerTransform->GetWorldPos() };

		pointerTransform->Translate(currentWorldPos.x, nextTransform->GetWorldPos().y - 50.0f, currentWorldPos.z);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
	{
		if(m_CurrentSelectedPlayerStyle == 0)
		{
			// Initialize Single play
			//game->playStyle = PlayStyle::Solo;
			return std::make_shared<PlayState>();
		}
		else
		{
			// Initialize Coop play
			//game->playStyle = PlayStyle::Coop;
			//game->saveState = std::make_shared<PlayState>();
			 //GameObject::Create(world);

			return std::make_shared<PlayState>();
		}
	}
	
	
	return {};
}

void MenuState::Enter(SLDWorldEntity& world, GameStateComponent*)
{
	using namespace SLD;

	const auto& resourceManger{ Instance<ResourceManager>() };
	
	sf::Font* font{ Instance<ResourceManager>()->Get<sf::Font>(GalagaScene::MainFont) };
	sf::Texture* mainSprite{ resourceManger->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };

	if(font && mainSprite)
	{
		const std::string playOnString{ "1 Player" };
		const std::string playOnString2{ "2 Players" };
		const size_t fontSize{ 50 };

		const auto player1Object{ GameObject::Create(world) };
		m_Player1Text = player1Object->GetId();
		TextRenderComponent* test =  player1Object->AddComponent<TextRenderComponent>({ font,playOnString,fontSize });
		test;

		const auto& player1Transform{ player1Object->GetComponent<TransformComponent>() };

		const auto player2Object{ GameObject::Create(world) };
		m_Player2Text = player2Object->GetId();
		player2Object->AddComponent<TextRenderComponent>({ font,playOnString2,fontSize });

		player2Object->GetComponent<TransformComponent>()->Translate(0.0f, -float(fontSize), 1.0f);

		const Vector4<uint32_t> rect{289,136,16,16};

		const auto pointer{ GameObject::Create(world) };
		pointer->AddComponent<SpriteRenderComponent>({ *mainSprite,rect,{GalagaScene::GlobalScale} });
		m_Pointer = pointer->GetId();

		// set pointer to the first "1 Player"
		pointer->GetComponent<TransformComponent>()->Translate(-float(fontSize), player1Transform->GetWorldPos().y - float(fontSize), 1.0f);
	}
	

}

void MenuState::Exit(SLDWorldEntity& world, GameStateComponent*)
{
	for (const auto& system : m_ExistSystem)
	{
		world.RemoveSystem(system.lock());
	}

	world.DestroyGameObject(m_Player1Text);
	world.DestroyGameObject(m_Player2Text);
	world.DestroyGameObject(m_Pointer);
	
	m_ExistSystem.clear();
	
	m_Player1Text = 0;
	m_Player2Text = 0;
	m_Pointer = 0;
}

