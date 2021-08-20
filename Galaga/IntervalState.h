#pragma once
#include "GameState.h"

class IntervalState :
    public GameState
{
public:

	SharedPtr<GameState> HandleInput(SLDWorldEntity&, GameStateComponent*) override;
	void Update(SLDWorldEntity&, float, GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
	void Exit(SLDWorldEntity&, GameStateComponent*) override;

private:

	SLD::GameObjectId m_StageText{};
	SLD::GameObjectId m_ReadyText{};

	float m_DisplayTimer{};
	float m_MaxDisplayTime{};
	bool m_FinishedDisplay{};
};

