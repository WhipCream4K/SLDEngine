#include "QBertGame.h"
#include "QBertParams.h"

//QBertGame::QBertGame(const std::any& windowHandle)
//	: m_Framework()
//	, m_Player(m_Framework.GetDefaultWorldEntity())
//{
//	windowHandle;
//}

QBertGame::QBertGame(HWND windowHandle)
	: m_Framework()
	, m_Player(m_Framework.GetDefaultWorldEntity())
	, m_Level(m_Framework.GetDefaultWorldEntity())
{
	m_Framework.CreateViewPortFromHWND(windowHandle);
}

void QBertGame::Start()
{
	// Game Sprite Sheet
	const bool success{ m_QBertSprite.loadFromFile("./Resources/SpriteSheet/QBert_Sprites.png") };

	if(success)
	{
		// Player
		m_Player.SetSpriteTexture(m_QBertSprite);

		// Map generation
		m_Level.SetTexture(m_QBertSprite);
		m_Level.ChangeAllPlatformTextureRect(QBert::Level1::RectPlatFormStart);

		// HUD generation
	}



}

void QBertGame::Run()
{
	// input layer
	m_Framework.TranslateUserInputs();

	// World Updates


	// Async update and render
	m_Framework.Step();

}
