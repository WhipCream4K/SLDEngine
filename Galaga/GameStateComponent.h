#pragma once

//#include <SLDFramework.h>
//#include "../SLDFramework/Source/Components/BaseComponent.h"
#include <Components/BaseComponent.h>

enum class GameState
{
	Menu,
	Play,
	End,

	Count
};

struct GameStateComponent : SLD::ComponentT<GameStateComponent>
{
	GameStateComponent(GameState startState)
		: currentState(startState)
		, playStage()
		, shouldSpawn(true)
	{
	}

	GameState currentState;
	size_t playStage;
	bool shouldSpawn;
};

struct StateTag : SLD::ComponentT<StateTag>
{
	StateTag(GameState startState)
		: spawnState(startState)
	{
	}

	GameState spawnState;
};

