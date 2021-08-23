#pragma once
#include "GameState.h"

class EndState :
	public GameState
{
public:
	SharedPtr<GameState> HandleInput(SLDWorldEntity&, GameStateComponent*) override;
	void Update(SLDWorldEntity&, float, GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
	void Exit(SLDWorldEntity&, GameStateComponent*) override;

private:

	SLD::GameObjectId m_ResultText;
	SLD::GameObjectId m_ShotsFiredText;
	SLD::GameObjectId m_NumberOfHitsText;
	SLD::GameObjectId m_HitMissRatioText;

	float m_DisplayTimer{};
	float m_MaxDisplayTime{ 5.0f };
	bool m_FinishedDisplay{};
};

