#pragma once

#include <SLDFramework.h>

class EndState;
struct GameStateComponent;
class PlayState;
class IntervalState;
class GameState
{
public:

	static SharedPtr<IntervalState> Interval;
	static SharedPtr<EndState> End;
	static SharedPtr<GameState> Null;

	virtual ~GameState() = default;
	virtual SharedPtr<GameState> HandleInput(SLDWorldEntity&,GameStateComponent*) { return {}; }
	virtual void Update(SLDWorldEntity&,float, GameStateComponent*) {}
	virtual void Enter(SLDWorldEntity&, GameStateComponent*) {}
	virtual void Exit(SLDWorldEntity&, GameStateComponent*) {}

protected:

	std::vector<WeakPtr<SLD::SystemBase>> m_ExistSystem;
};

