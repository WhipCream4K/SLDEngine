#include "QBertGame.h"
#include "QBertParams.h"
#include "GameObject/GameObject.h"

//#include <SFML/Window.hpp>
//#include <SFML/Graphics.hpp>

//QBertGame::QBertGame(const std::any& windowHandle)
//	: m_Framework()
//	, m_Player(m_Framework.GetDefaultWorldEntity())
//{
//	windowHandle;
//}

QBertGame::QBertGame(HWND windowHandle)
	: m_Framework()
{
	m_Framework.CreateViewPortFromHWND(windowHandle);
}

void QBertGame::Start()
{
	using namespace SLD;

	m_Player = std::make_shared<Player>(m_Framework.GetDefaultWorldEntity());
	m_Level = std::make_shared<Level>(m_Framework.GetDefaultWorldEntity());
	
	// Game Sprite Sheet
	const bool success{ m_QBertSprite.loadFromFile("./Resources/SpriteSheet/QBert_Sprites.png") };

	if (success)
	{
		// Player
		m_Player->SetSpriteTexture(m_QBertSprite);

		// Map generation
		m_Level->SetTexture(m_QBertSprite);
		m_Level->ChangeAllPlatformTextureRect(QBert::Level1::RectPlatFormStart);

		// HUD generation
	}

	// Binding

	InputSetting& gameInput{ m_Framework.GetDefaultWorldEntity().GetWorldInputSetting() };
	
	gameInput.AddAxisMapping("Horizontal", {
		AxisKey{Key{InputDevice::D_Keyboard,sf::Keyboard::A},-1.0f},
		AxisKey{Key{InputDevice::D_Keyboard,sf::Keyboard::D},1.0f}
	});

	gameInput.AddActionMapping("MoveDiagonal",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::H}}
		});
	
	m_Player->SetUpPlayerInput();
}

void QBertGame::Run()
{
	// input layer
	if(!m_Framework.TranslateUserInputs())
	{
		const float deltaTime{ m_Framework.GetDefaultWorldEntity().GetDeltaTime() };

		// World Updates
		m_Player->Update(deltaTime);

		//if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		//{
		//	auto& currentTransform{ m_Player.GetGameObject()->GetTransform() };
		//	const auto& currPos{ currentTransform->GetPtr()->GetWorldPos() };
		//	float newPosX{ currPos.x + (1000.0f * 1.0f * m_Framework.GetDefaultWorldEntity().GetDeltaTime()) };
		//	currentTransform->GetPtr()->Translate(newPosX, currPos.y, currPos.z);
		//}


		// Async update and render
		m_Framework.Step();
	}
}
