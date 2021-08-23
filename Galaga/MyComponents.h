#pragma once

#include <SFML/Graphics.hpp>
#include <Components/BaseComponent.h>

#include "OnObjectHit.h"

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

struct TextRenderComponent : SLD::ComponentT<TextRenderComponent>
{

	TextRenderComponent(sf::Font* font, const std::string& string, size_t fontSizePixel)
		: text(string)
		, font(font)
		, fontSize(fontSizePixel)
		, color(255, 255, 255)
	{
	}

	std::string text;
	sf::Font* font;
	sf::Color color;
	size_t fontSize;
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
		, maxHealth(startHealth)
	{
	}

	int health{};
	int maxHealth{};
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

struct DiveComponent : SLD::ComponentT<DiveComponent>
{
	DiveComponent()
		: currentWayPoint()
		, initialPosition()
	{
	}

	int currentWayPoint;
	rtm::float3f initialPosition;
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

enum class EnemyStateNums
{
	FlyIn,
	Formation,
	ReadyToDive,
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

struct BreakawayTendency
{
	BreakawayTendency(float breakPercent)
		: percent(breakPercent)
	{
	}

	float percent;
};

struct EnemyTag : SLD::ComponentT<EnemyTag>
{
	EnemyTag(EnemyType setType)
		: type(setType)
		, state(EnemyStateNums::FlyIn)
	{
	}

	EnemyType type;
	EnemyStateNums state;
};

struct ScoreComponent : SLD::ComponentT<ScoreComponent>
{
	ScoreComponent(int score)
		: value(score)
	{
	}

	int value;
};

//class OnObjectHit;
struct OnHitCommand : SLD::ComponentT<OnHitCommand>
{
	OnHitCommand(const SharedPtr<OnObjectHit>& hit)
		: onHit(hit)
	{
	}
	
	SharedPtr<OnObjectHit> onHit;
};

//struct TendencyToShoot : SLD::ComponentT<TendencyToShoot>
//{
//	TendencyToShoot(int percent)
//		: value(percent)
//	{
//	}
//
//	int value;
//};
//
//struct TendencyToDive : SLD::ComponentT<TendencyToDive>
//{
//	TendencyToDive(int percent)
//		: value(percent)
//	{
//	}
//
//	int value;
//};

struct PlayerTag : SLD::ComponentT<PlayerTag>
{

};
