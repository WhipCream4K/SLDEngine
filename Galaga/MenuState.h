#pragma once

#include "GameState.h"

class MenuState : public GameState
{
public:

	MenuState();
	SharedPtr<GameState> HandleInput(SLDWorldEntity&,GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
	void Exit(SLDWorldEntity&, GameStateComponent*) override;

private:

	
	SLD::GameObjectId m_Player1Text{};
	SLD::GameObjectId m_Player2Text{};
	SLD::GameObjectId m_Pointer{};
	
	size_t m_MaxPlayerCount;
	size_t m_CurrentSelectedPlayerStyle{};
};

