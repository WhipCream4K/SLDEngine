#pragma once
#include "PlayState.h"

class SoloPlayState :
    public PlayState
{
public:

	SharedPtr<GameState> HandleInput(SLDWorldEntity&, GameStateComponent*) override;
	void Update(SLDWorldEntity&, float, GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
};

