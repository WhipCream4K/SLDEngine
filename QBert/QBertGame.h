#pragma once

#include <SLDFramework.h>
#include <any>

#include "Player.h"
#include "Level.h"

class QBertGame
{
public:
	
	//QBertGame(const std::any& windowHandle);
	QBertGame(HWND windowHandle);

	void Start();
	void Run();

private:
	
	SLDFramework m_Framework;

	// Game Data
	Player m_Player;
	Level m_Level;

	// Texture Resource
	sf::Texture m_QBertSprite;
};

