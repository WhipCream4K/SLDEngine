#include "SoloPlayState.h"

void SoloPlayState::Update(SLDWorldEntity& world, float dt, GameStateComponent* game)
{
	PlayState::Update(world, dt, game);

	// Display UI
	
}

void SoloPlayState::Enter(SLDWorldEntity& world, GameStateComponent* game)
{
	PlayState::Enter(world, game);

	// Instantiate one player
	
}
