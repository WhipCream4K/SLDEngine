#include "GameState.h"
#include "PlayState.h"
#include "IntervalState.h"
#include "EndState.h"

//SharedPtr<PlayState> GameState::Play{ std::make_shared<PlayState>() };
SharedPtr<EndState> GameState::End{std::make_shared<EndState>()};
SharedPtr<IntervalState> GameState::Interval{ std::make_shared<IntervalState>() };
SharedPtr<GameState> GameState::Null{ std::make_shared<GameState>() };