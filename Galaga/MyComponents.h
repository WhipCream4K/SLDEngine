#pragma once

#include <SFML/Graphics.hpp>
#include <Components/BaseComponent.h>
//#include <Math/RealTimeMath.h>

struct SpriteRenderComponent : SLD::ComponentT<SpriteRenderComponent>
{
	SpriteRenderComponent(const SharedPtr<sf::Texture>& texture, const SLD::Vector4<uint32_t>& rect,
		const rtm::float2f& scale,
		const rtm::float2f& offset = { 0.5f,0.5f })
		: sprite(*texture, sf::IntRect{
					 int(rect.x),
					 int(rect.y),
					 int(rect.z),
					 int(rect.a) })
					 , spriteOffset(offset)
		, scaleVec(scale)
	{
	}

	SpriteRenderComponent(const sf::Texture& texture,
		const SLD::Vector4<uint32_t>& rect,
		const rtm::float2f& scale,
		const rtm::float2f& offset = { 0.5f,0.5f })
		: sprite(texture, sf::IntRect{
					 int(rect.x),
					 int(rect.y),
					 int(rect.z),
					 int(rect.a) })
					 , spriteOffset(offset)
		, scaleVec(scale)
	{
	}

	sf::Sprite sprite{};
	rtm::float2f spriteOffset{};
	rtm::float2f scaleVec{};
	bool shouldRender{ true };
};

struct InputListener : SLD::ComponentT<InputListener>
{
	InputListener(size_t index)
		: playerIndex(index)
	{
	}

	size_t playerIndex{};
};

struct HealthComponent : SLD::ComponentT<HealthComponent>
{
	HealthComponent(int startHealth)
		: health(startHealth)
	{
	}

	int health{};
};

enum class SpawnDirection
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,

	Count
};

struct FlyInComponent : SLD::ComponentT<FlyInComponent>
{
	FlyInComponent(const std::vector<rtm::float2f>& path)
		: carryPath(path)
		, spawnDirection()
		, currentWayPoint()
	{
	}

	const std::vector<rtm::float2f>& carryPath;
	SpawnDirection spawnDirection;
	int currentWayPoint;
};

struct FormationComponent : SLD::ComponentT<FormationComponent>
{
	FormationComponent(int startRow, int startCol)
		: row(startRow)
		, col(startCol)
		, alreadyInside()
	{
	}

	int row{};
	int col{};
	bool alreadyInside{};
};

struct SpeedComponent : SLD::ComponentT<SpeedComponent>
{
	SpeedComponent(float speed)
		: value(speed)
	{
	}

	float value;
};

struct ProjectileComponent : SLD::ComponentT<ProjectileComponent>
{
	ProjectileComponent(float startForce, bool spawnActive = true)
		: impulseForce(startForce)
		, isActive(spawnActive)
	{
	}

	float impulseForce{};
	bool isActive{};
};

struct LineComponent : SLD::ComponentT<LineComponent>
{
	LineComponent()
	{
	}

	std::vector<rtm::float2f> points;
};

enum class EnemyType
{
	Zako,
	Goei,
	Galagas
};

enum class EnemyState
{
	FlyIn,
	Formation,
	Dive,
	Died
};

struct ParticleComponent : SLD::ComponentT<ParticleComponent>
{
	ParticleComponent(float activeTime)
		: lifeTime(activeTime)
		, isActive(true)
	{
	}

	float lifeTime{};
	bool isActive{};
};

struct EnemyTag : SLD::ComponentT<EnemyTag>
{
	EnemyTag(EnemyType setType)
		: type(setType)
		, state(EnemyState::FlyIn)
	{
	}

	EnemyType type;
	EnemyState state;
};

struct PlayerTag : SLD::ComponentT<PlayerTag>
{

};