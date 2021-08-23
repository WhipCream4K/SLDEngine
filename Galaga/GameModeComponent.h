#pragma once

#include <SLDFramework.h>

#include "GameStateComponent.h"

class GameModeComponent : SLD::ComponentT<GameModeComponent>
{
public:

	size_t currentSpawnPlayer;
	PlayStyle currentPlayStyle;
	bool isInActive{};
	bool shouldRestartGame{ true };
};

