#pragma once

//#include <SLDFramework.h>
//#include "../SLDFramework/Source/Components/BaseComponent.h"
#include <Components/BaseComponent.h>
//#include "MenuState.h"
#include "HUD.h"

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
	GameStateComponent(int maxEnemy, int maxStageCnt, int maxPlayerLives, size_t playerIdx)
		: state()
		, playerIndex(playerIdx)
		, currentEnemyDead()
		, maxEnemyCount(maxEnemy)
		, currentStageCnt()
		, currentPlayerLives(maxPlayerLives)
		, maxStage(maxStageCnt)
		, isPlayerDead()
	{
	}

	std::vector<SLD::GameObjectId> activeEnemies;
	SharedPtr<GameState> state; // Dangerous
	SLD::GameObjectId parent{};
	size_t playerIndex;
	SLD::GameObjectId playerId{};
	HUD attachedHUD{};
	int currentEnemyDead;
	int maxEnemyCount;
	int currentStageCnt;
	int currentPlayerLives;
	int maxStage{};
	int shotCount{};
	int currentScore{};
	int maxZakoDive{ 1 };
	int maxGoeiDive{ 1 };
	bool isPlayerSet{};
	bool isFormationLocked{};
	bool isStageStart{};
	bool isActive{};
	bool isPlayerDead{};
	bool isInMidStage{};
	bool isHUDInitialized{};
};

