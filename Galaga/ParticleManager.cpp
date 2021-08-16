#include "ParticleManager.h"
#include "GalagaScene.h"
#include "Particle.h"
#include "Helpers/utils.h"

ParticleManager::ParticleManager()
{
	m_ParticleRect[size_t(ParticleType::EnemyDead)] = {391,1,32,32};
	m_ParticleRect[size_t(ParticleType::PlayerDead)] = {247,1,32,32};
}

void ParticleManager::Spawn(SLD::WorldEntity& world,const rtm::float3f& spawnPoint, ParticleType type)
{
	using namespace SLD;
	
	rtm::float3f adjustedSpawnPoint{ spawnPoint };
	adjustedSpawnPoint.z = float(GalagaScene::Layer::Particle);

	InstantiatePrefab<Particle>(world, {m_ParticleRect[size_t(type)]}, adjustedSpawnPoint);
}
