#pragma once
#include "GameState.h"

class IntervalState :
    public GameState
{
public:

	IntervalState() = default;
	SharedPtr<GameState> HandleInput(SLDWorldEntity&, GameStateComponent*) override;
	void Update(SLDWorldEntity&, float, GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
	void Exit(SLDWorldEntity&, GameStateComponent*) override;

private:

	void InitializedHUD(SLD::WorldEntity&,GameStateComponent*);

	SLD::GameObjectId m_StageText{};
	SLD::GameObjectId m_ReadyText{};

	float m_DisplayTimer{};
	float m_MaxDisplayTime{5.0f};
	bool m_FinishedDisplay{};
};

