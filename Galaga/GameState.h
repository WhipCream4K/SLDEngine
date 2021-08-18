#pragma once

#include <SLDFramework.h>

class GameState
{
public:

	virtual void HandleInput(SLD::InputSettings& input) = 0;
	virtual void Update(SLD::WorldEntity& world) = 0;
};