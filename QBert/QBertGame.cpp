#include "QBertGame.h"
#include "QBertParams.h"
#include "GameObject/GameObject.h"
//#include <SFML/Window.hpp>

//#include <SFML/Window.hpp>
//#include <SFML/Graphics.hpp>

//QBertGame::QBertGame(const std::any& windowHandle)
//	: m_Framework()
//	, m_Player(m_Framework.GetDefaultWorldEntity())
//{
//	windowHandle;
//}

QBertGame::FGameWon QBertGame::OnGameWon{};
const rtm::float3f QBertGame::Player1DefaultSpawnPoint{
	0.0f,
	(64.0f + QBert::LevelPixelY / 2.0f) * QBert::GlobalScaleY,
	float(QBert::Layer::Player) };

QBertGame::QBertGame(HWND windowHandle)
	: m_Framework()
{
	m_Framework.CreateViewPortFromHWND(windowHandle);

	//Player1DefaultSpawnPoint = rtm::float3f{
	//	0.0,(64.0f + QBert::LevelPixelY / 2.0f) * QBert::GlobalScaleY,
	//	float(QBert::Layer::Player) };
}

void QBertGame::Start()
{
	using namespace SLD;

	// Game Sprite Sheet
	const bool success{ m_QBertSprite.loadFromFile("./Resources/SpriteSheet/QBert_Sprites.png") };

	if (success)
	{
		// Create Sprite
		//const int totalSpriteForNow{ 3 };
		//SetUpSprite(totalSpriteForNow, m_QBertSprite, QBert::Level1::SpriteStart);

		m_Level = std::make_shared<Level>(m_Framework.GetDefaultWorldEntity());
		m_Player = std::make_shared<Player>(m_Framework.GetDefaultWorldEntity(),m_Level);
		
		// Player
		m_Player->SetSpriteTexture(m_QBertSprite);
		m_Player->SetCurrentNode(0, 0);
		m_Player->SetPos(Player1DefaultSpawnPoint);

		// Map generation
		m_Level->SetTexture(m_QBertSprite);

		m_Level->ChangeLevelSprite(Level::LevelState::Level1);
		m_Level->SetSpriteOrigin(QBert::Level1::SpriteStart);
		//m_Level->SetSpriteTextureRectFromId(0, QBert::Level1::SpriteStart);
		//m_Level->SetSpriteTextureRectFromId(1, QBert::Level1::SpriteEnd);
		//m_Level->SetSpriteTextureRectFromId(2, QBert::Level1::SpriteSpare);

		m_Level->ChangeAllPlatformSprite(0);

		Player::OnPlayerFinishedJump.AddDynamic(&Level::OnPlayerFinishedJump, m_Level);
		Player::OnPlayerDied.AddDynamic(&Level::OnPlayerDied, m_Level);
		Level::OnLevelChange.AddDynamic(&Player::OnLevelChange, m_Player);
		QBertGame::OnGameWon.AddDynamic(&Level::OnGameWon, m_Level);
		QBertGame::OnGameWon.AddDynamic(&Player::OnGameClear, m_Player);
		
		// HUD generation
	}

	// Binding
	InputSetting& gameInput{ m_Framework.GetDefaultWorldEntity().GetWorldInputSetting() };

	gameInput.AddAxisMapping("Horizontal", {
		AxisKey{Key{InputDevice::D_Keyboard,sf::Keyboard::A},-1.0f},
		AxisKey{Key{InputDevice::D_Keyboard,sf::Keyboard::D},1.0f}
		});

	gameInput.AddActionMapping("MoveUpRight",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::E}}
		});

	gameInput.AddActionMapping("MoveUpLeft",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::Q}}
		});

	gameInput.AddActionMapping("MoveDownLeft",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::Z}}
		});

	gameInput.AddActionMapping("MoveDownRight",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::C}}
		});

	m_Player->SetUpPlayerInput();
}

void QBertGame::Run()
{
	// input layer
	if (!m_Framework.TranslateUserInputs())
	{
		const float deltaTime{ m_Framework.GetDefaultWorldEntity().GetDeltaTime() };

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F9))
			OnGameWon.BroadCast();
		
		// World Updates
		m_Player->Update(deltaTime);
		m_Level->Update(deltaTime);

		// Async update and render
		m_Framework.Step();
	}
}

//void QBertGame::SetUpSprite(int count,const sf::Texture& texture, const sf::IntRect& textureRect)
//{
//	for (int i = 0; i < count; ++i)
//	{
//		auto ref{ m_MainSprites.emplace_back(std::make_shared<sf::Sprite>()) };
//		ref->setTexture(texture);
//		ref->setTextureRect(textureRect);
//		ref->setOrigin(float(textureRect.width) * 0.5f, float(textureRect.height) * 0.5f);
//	}
//}
