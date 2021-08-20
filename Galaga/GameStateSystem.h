#pragma once

#include <System/SystemTemplate.h>
#include "GameStateComponent.h"
#include "MyComponents.h"

class Player;
class GameStateSystem : public SLD::SystemTemplate<
	GameStateComponent,
	InputListener>
{
public:
	
	
	GameStateSystem(SLD::WorldEntity& world);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, GameStateComponent*, InputListener*) override;

private:

	void HandleMenuState(GameStateComponent* game);
	void HandlePlayState(GameStateComponent* game);

	
	std::array<bool, size_t(GameStateNum::Count)> m_StateBits;
	std::array<std::vector<WeakPtr<SystemBase>>, size_t(GameStateNum::Count)> m_SystemStateArray;
	size_t m_PlayerCount;
};

