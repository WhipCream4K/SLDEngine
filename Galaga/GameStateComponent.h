#pragma once

//#include <SLDFramework.h>
//#include "../SLDFramework/Source/Components/BaseComponent.h"
#include <Components/BaseComponent.h>
#include "MenuState.h"

enum class GameStateNum
{
	Menu,
	Play,
	End,

	Count
};

enum class PlayStyle
{
	Solo,
	Coop
};

class GameState;
struct GameStateComponent : SLD::ComponentT<GameStateComponent>
{
	GameStateComponent(int maxEnemy,int maxStageCnt)
		: state()
		, currentEnemyCount()
		, maxEnemyCount(maxEnemy)
		, currentStageCnt()
		, maxStage(maxStageCnt)
		, playStyle()
		, playerDead()
	{
	}

	SharedPtr<GameState> state;
	int currentEnemyCount;
	int maxEnemyCount;
	int currentStageCnt;
	int maxStage;
	PlayStyle playStyle;
	bool playerDead;

	//GameStateNum state;
	//size_t playStage;
	//bool shouldSpawn;
};

