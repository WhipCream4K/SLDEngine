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
	, m_Player1DefaultSpawnPoint()
{
	m_Framework.CreateViewPortFromHWND(windowHandle);

	m_Player1DefaultSpawnPoint = rtm::float3f{
		0.0,(64.0f + QBert::LevelPixelY / 2.0f) * QBert::GlobalScaleY,
		float(QBert::Layer::Player) };
}

void QBertGame::Start()
{
	using namespace SLD;

	m_Level = std::make_shared<Level>(m_Framework.GetDefaultWorldEntity());
	m_Player = std::make_shared<Player>(m_Framework.GetDefaultWorldEntity(),m_Level);

	// Game Sprite Sheet
	const bool success{ m_QBertSprite.loadFromFile("./Resources/SpriteSheet/QBert_Sprites.png") };

	if (success)
	{
		// Player
		m_Player->SetSpriteTexture(m_QBertSprite);
		m_Player->SetCurrentNode(0, 0);
		m_Player->SetPos(m_Player1DefaultSpawnPoint);

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

		// World Updates
		m_Player->Update(deltaTime);

		// Async update and render
		m_Framework.Step();
	}
}
