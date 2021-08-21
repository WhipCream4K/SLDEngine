#pragma once

#include "GameState.h"
#include "SpawnData.h"

#include "SpawnPaths.h"

class PlayState : public GameState
{
public:

	
	SharedPtr<GameState> HandleInput(SLDWorldEntity&,GameStateComponent*) override;
	void Update(SLDWorldEntity&, float, GameStateComponent*) override;
	void Enter(SLDWorldEntity&, GameStateComponent*) override;
	void Exit(SLDWorldEntity&, GameStateComponent*) override;

	static constexpr float TrainInterval{ 0.3f };

private:

	void SpawnEnemy(SLD::WorldEntity& world, float dt,int stage);
	void SpawnEnemySection(SLD::WorldEntity& ,float dt, const SpawnData& section);
	void SignalEnemyLockedFormation(SLD::WorldEntity& world);

	std::vector<SLD::GameObjectId> m_SpawnEnemies;

	const StageSpawnData* m_pSpawnData{};
	const SpawnPaths* m_pSpawnPaths{};
	
	SLD::GameObjectId m_FormationTracker{};
	//SLD::GameObjectId m_ReadyText{};
	
	//float m_MaxDisplayTime{};
	//float m_DisplayTimer{};

	float m_WaveTimer;
	float m_SpawnTimer;

	size_t m_BusCounter;
	size_t m_SectionCounter;
	
	//bool m_DisplayInterval{};
	bool m_IsSpawning;
	bool m_ShouldRestart;
};

