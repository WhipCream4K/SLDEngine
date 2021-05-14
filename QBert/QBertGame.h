#pragma once

#include <SLDFramework.h>
#include "Player.h"

#include <any>
class QBertGame
{
public:
	
	QBertGame(const std::any& windowHandle);
	QBertGame(HWND windowHandle);

	void Start();
	void Run();

private:
	
	SLDFramework m_Framework;

	// Game Data
	Player m_Player;

	// Texture Resource
	sf::Texture m_QBertSprite;
};

