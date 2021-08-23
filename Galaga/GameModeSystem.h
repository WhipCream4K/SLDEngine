#pragma once

#include <SLDFramework.h>
#include <System/SystemTemplate.h>
#include "GameModeComponent.h"

class ReturnToMenu;
class GameModeSystem : public SLD::SystemTemplate<GameModeComponent>
{
public:

	GameModeSystem(SLDWorldEntity& world);

	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, GameModeComponent*) override;
	void OnReturnToMenu(bool);

private:

	void SpawnMenu();
	SLD::GameObjectId SpawnFormationTracker();
	SLD::GameObjectId SpawnGameState(size_t playerIdx);
	void Clear();

	SLD::GameObjectId m_Player1Text{};
	SLD::GameObjectId m_Player2Text{};
	SLD::GameObjectId m_Pointer{};

	SLD::GameObjectId m_GameState1;
	SLD::GameObjectId m_GameState2;


	SharedPtr<ReturnToMenu> m_OnReturnToMenu;
	size_t m_MaxPlayerCount{ 2 };
	size_t m_CurrentSelectedPlayerStyle{};

};

