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

	//void SetUpSprite(int count,const sf::Texture& texture,const sf::IntRect& textureRect);
	
	SLDFramework m_Framework;

	// Game Data
	RefPtr<Player> m_Player;
	RefPtr<Level> m_Level;

	rtm::float3f m_Player1DefaultSpawnPoint;
	
	// Texture Resource
	sf::Texture m_QBertSprite;
	//std::vector<RefPtr<sf::Sprite>> m_MainSprites;
};

