#include "QBertGame.h"
#include "QBertParams.h"
#include "GameObject/GameObject.h"
#include "HUD.h"
#include "Player.h"
#include "Level.h"

//QBertGame::QBertGame(const std::any& windowHandle)
//	: m_Framework()
//	, m_Player(m_Framework.GetDefaultWorldEntity())
//{
//	windowHandle;
//}

QBertGame::FGameWon QBertGame::OnGameWon{};
QBertGame::FAddScore QBertGame::OnAddScore{};

const rtm::float3f QBertGame::Player1DefaultSpawnPoint{
	0.0f,
	(64.0f + QBert::LevelPixelY / 2.0f) * QBert::GlobalScaleY,
	float(QBert::Layer::Player) };

QBertGame::QBertGame(HWND windowHandle)
	: m_Framework()
{
	m_Framework.CreateViewPortFromHWND(windowHandle);
}

void QBertGame::Start()
{
	using namespace SLD;

	// Game Sprite Sheet
	bool success{ m_QBertSprite.loadFromFile("./Resources/SpriteSheet/QBert_Sprites.png") };
	success = m_QBertFont.loadFromFile("./Resources/Fonts/Minecraft.ttf");

	if (success)
	{
		auto& defaultWorld{ m_Framework.GetDefaultWorldEntity() };
		
		m_Level = std::make_shared<Level>(defaultWorld);
		m_Player = std::make_shared<Player>(defaultWorld,m_Level);
		m_HUD = std::make_shared<HUD>(defaultWorld);
		
		// Player
		m_Player->SetSpriteTexture(m_QBertSprite);
		m_Player->SetCurrentNode(0, 0);
		m_Player->SetPos(Player1DefaultSpawnPoint);

		// Map generation
		m_Level->SetTexture(m_QBertSprite);

		m_Level->ChangeLevelSprite(Level::LevelState::Level1);
		m_Level->SetSpriteOrigin(QBert::Level1::SpriteStart);
		m_Level->ChangeAllPlatformSprite(0);

		
		Player::OnPlayerFinishedJump.AddDynamic(&Level::OnPlayerFinishedJump, m_Level);
		Player::OnPlayerDied.AddDynamic(&Level::OnPlayerDied, m_Level);
		Level::OnLevelChange.AddDynamic(&Player::OnLevelChange, m_Player);
		Level::OnLevelChange.AddDynamic(&HUD::OnLevelChange, m_HUD);
		QBertGame::OnAddScore.AddDynamic(&HUD::ShouldAddScore, m_HUD);
		QBertGame::OnGameWon.AddDynamic(&Level::OnGameWon, m_Level);
		QBertGame::OnGameWon.AddDynamic(&Player::OnGameClear, m_Player);
		
		// HUD generation
		m_HUD->SetTexture(m_QBertSprite);
		m_HUD->SetFont(m_QBertFont);
	}

	// Binding
	InputSetting& gameInput{ m_Framework.GetDefaultWorldEntity().GetWorldInputSetting() };

	gameInput.AddAxisMapping("Horizontal", {
		AxisKey{Key{InputDevice::D_Keyboard,sf::Keyboard::A},-1.0f},
		AxisKey{Key{InputDevice::D_Keyboard,sf::Keyboard::D},1.0f}
		});

	gameInput.AddActionMapping("MoveUpRight",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::E}},
			ActionKey{Key{InputDevice::D_Gamepad,uint16_t(GamePadKey::GPK_DPAD_Up)}}
		});

	gameInput.AddActionMapping("MoveUpLeft",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::Q}},
			ActionKey{Key{InputDevice::D_Gamepad,uint16_t(GamePadKey::GPK_DPAD_Left)}}
		});

	gameInput.AddActionMapping("MoveDownLeft",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::Z}},
			ActionKey{Key{InputDevice::D_Gamepad,uint16_t(GamePadKey::GPK_DPAD_Down)}}
		});

	gameInput.AddActionMapping("MoveDownRight",
		{
			ActionKey{Key{InputDevice::D_Keyboard,sf::Keyboard::C}},
			ActionKey{Key{InputDevice::D_Gamepad,uint16_t(GamePadKey::GPK_DPAD_Right)}}
		});

	m_Player->SetUpPlayerInput();
}

void QBertGame::Run()
{
	// input layer
	if (!m_Framework.TranslateUserInputs())
	{
		const float deltaTime{ m_Framework.GetDefaultWorldEntity().GetDeltaTime() };

		// Skip
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F9))
			OnGameWon.BroadCast();
		
		// World Updates
		m_Player->Update(deltaTime);
		m_Level->Update(deltaTime);
		m_HUD->Update(deltaTime);

		// Async update and render
		m_Framework.Step();
	}
}
