#pragma once

#include "GameState.h"
#include "SpawnData.h"

#include "SpawnPaths.h"

class PlayState : public GameState
{
public:

	SharedPtr<GameState> HandleInput(SLDWorldEntity&, GameStateComponent*) override;
	void Update(SLDWorldEntity&, float, GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
	void Exit(SLDWorldEntity&, GameStateComponent*) override;

	static constexpr float TrainInterval{ 0.3f };

private:

	void SpawnEnemy(SLD::WorldEntity& world, float dt, int stage, GameStateComponent* game);
	void SpawnEnemySection(SLD::WorldEntity&, float dt, const SpawnData& section, GameStateComponent* game);
	void SignalEnemyLockedFormation(SLD::WorldEntity& world, GameStateComponent* game);
	void RandomizeDive(SLD::WorldEntity& world, float dt, GameStateComponent* game);
	void SpawnPlayer(SLD::WorldEntity& world, GameStateComponent* game);
	void Clear(SLD::WorldEntity& world, GameStateComponent* game);

	const StageSpawnData* m_pSpawnData{};
	const SpawnPaths* m_pSpawnPaths{};

	//SLD::GameObjectId m_ReadyText{};

	//float m_MaxDisplayTime{};
	//float m_DisplayTimer{};

	float m_WaveTimer{};
	float m_SpawnTimer{};

	float m_DiveTimer{};
	float m_DiveMaxTimer{ 4.0f };

	size_t m_BusCounter{};
	size_t m_SectionCounter{};

	bool m_IsSpawning{};
	bool m_Test{};
	std::future<void> m_WaitForSignal{};
};

