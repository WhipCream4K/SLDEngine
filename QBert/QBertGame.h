#pragma once

#include <SLDFramework.h>
#include <any>
#include "Core/DynamicMulticastDelegate.h"

//#include "Player.h"
//#include "Level.h"

class Player;
class Level;
class HUD;
class QBertGame
{
public:
	
	//QBertGame(const std::any& windowHandle);
	QBertGame(HWND windowHandle);

	void Start();
	void Run();
	using FGameWon = SLD::DynamicMulticastDelegate<void()>;
	using FAddScore = SLD::DynamicMulticastDelegate<void(int)>;
	static FAddScore OnAddScore;
	static FGameWon OnGameWon;
	static const rtm::float3f Player1DefaultSpawnPoint;

private:
	
	SLDFramework m_Framework;
	
	// Game Data
	RefPtr<Player> m_Player;
	RefPtr<Level> m_Level;
	RefPtr<HUD> m_HUD;
	
	// Texture Resource
	sf::Texture m_QBertSprite;
	sf::Font m_QBertFont;
};

