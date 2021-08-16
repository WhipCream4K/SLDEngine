#pragma once

#include <Core/Singleton.h>

namespace SLD {
	class GameObject;
	class WorldEntity;
}

namespace sf {
	class Sprite;
}

enum class ParticleType
{
	PlayerDead,
	EnemyDead,

	Count
};

class ParticleManager : public SLD::Singleton<ParticleManager>
{
public:
	
	
	static constexpr size_t MaxNumParticles{ 10 };

	friend class Singleton<ParticleManager>;

	ParticleManager();

	void Spawn(SLD::WorldEntity& world, const rtm::float3f& spawnPos,ParticleType type);

private:

	std::array<SLD::Vector4<uint32_t>, size_t(ParticleType::Count)> m_ParticleRect;
	std::array<SharedPtr<SLD::GameObject>, MaxNumParticles> m_ParticleInstances;
};

