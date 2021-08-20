#include "GameState.h"
#include "PlayState.h"
#include "IntervalState.h"

SharedPtr<PlayState> GameState::Play{ std::make_shared<PlayState>() };
SharedPtr<IntervalState> GameState::Interval{ std::make_shared<IntervalState>() };