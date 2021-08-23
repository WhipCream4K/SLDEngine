#include "EnemyManager.h"
#include <Helpers/utils.h>
#include "EnemyVariable.h"
#include "Zako.h"
#include "Goei.h"
#include "Galagas.h"
#include <Physics/Components/Box2DComponent.h>
#include <Random/Random.h>
//#include "MyComponents.h"

EnemyManager::EnemyManager()
	: m_EnemyRowColFormation()
	, m_EnemyInstances()
{
	m_MaxEnemyCounter[EnemyType::Zako] = ZakoCntLimit;
	m_MaxEnemyCounter[EnemyType::Goei] = GoeiCntLimit;
	m_MaxEnemyCounter[EnemyType::Galagas] = GalagasCntLimit;

	InitializeRowCol();
}

SLD::GameObjectId EnemyManager::Spawn(SLD::WorldEntity& world, const rtm::float3f& pos, EnemyType type, SpawnDirection dir)
{
	using namespace SLD;

	size_t& activeEnemyCnt{ m_ActiveEnemyCounter[type] };
	const size_t maxEnemyCnt{ m_MaxEnemyCounter[type] };
	const auto& [row, col] = m_EnemyRowColFormation[type][activeEnemyCnt];
	SharedPtr<GameObject> instance{};
	SLD::GameObjectId out{};

	if (activeEnemyCnt < maxEnemyCnt)
	{
		switch (type)
		{
		case EnemyType::Zako:
		{
			SharedPtr<Zako> zako{};
			InstantiatePrefab<Zako>(world, { dir,row,col }, pos, zako);

			m_EnemyInstances[type].emplace_back(zako->GetGameObjectInstance());
			instance = zako->GetGameObjectInstance();
		}
		break;
		case EnemyType::Goei:
		{
			SharedPtr<Goei> goei{};
			InstantiatePrefab<Goei>(world, { dir,row,col }, pos, goei);

			m_EnemyInstances[type].emplace_back(goei->GetGameObject());

			instance = goei->GetGameObject();
		}
		break;
		case EnemyType::Galagas:
		{
			SharedPtr<Galagas> galagas{};
			InstantiatePrefab<Galagas>(world, { dir,row,col }, pos, galagas);

			m_EnemyInstances[type].emplace_back(galagas->GetGameObject());

			instance = galagas->GetGameObject();
		}

		break;
		default: break;
		}

		activeEnemyCnt++;
	}
	else
	{
		instance = m_EnemyInstances[type][activeEnemyCnt % maxEnemyCnt];
		instance->GetComponent<TransformComponent>()->Translate(pos);
		FlyInComponent* flyIn{ instance->GetComponent<FlyInComponent>() };
		if(flyIn)
		{
			flyIn->spawnDirection = dir;
		}
	}

	out = instance->GetId();

	return out;

}

SLD::GameObjectId EnemyManager::Spawn(SLD::WorldEntity& world, const rtm::float3f& pos, EnemyType type,
	SpawnDirection dir, EnemyStateNums state)
{
	using namespace SLD;

	size_t& activeEnemyCnt{ m_ActiveEnemyCounter[type] };
	const size_t maxEnemyCnt{ m_MaxEnemyCounter[type] };
	const auto& [row, col] = m_EnemyRowColFormation[type][activeEnemyCnt % maxEnemyCnt];
	SharedPtr<GameObject> instance{};
	SLD::GameObjectId out{};

	if (activeEnemyCnt < maxEnemyCnt)
	{
		switch (type)
		{
		case EnemyType::Zako:
		{
			SharedPtr<Zako> zako{};
			InstantiatePrefab<Zako>(world, { dir,row,col }, pos, zako);

			m_EnemyInstances[type].emplace_back(zako->GetGameObjectInstance());

			instance = zako->GetGameObjectInstance();
		}
		break;
		case EnemyType::Goei:
		{
			SharedPtr<Goei> goei{};
			InstantiatePrefab<Goei>(world, { dir,row,col }, pos, goei);

			m_EnemyInstances[type].emplace_back(goei->GetGameObject());

			instance = goei->GetGameObject();
		}
		break;
		case EnemyType::Galagas:
		{
			SharedPtr<Galagas> galagas{};
			InstantiatePrefab<Galagas>(world, { dir,row,col }, pos, galagas);

			m_EnemyInstances[type].emplace_back(galagas->GetGameObject());

			instance = galagas->GetGameObject();
		}

		break;
		default: break;
		}

		activeEnemyCnt++;
	}
	else
	{
		instance = m_EnemyInstances[type][activeEnemyCnt % maxEnemyCnt];
		instance->GetComponent<TransformComponent>()->Translate(pos);
		FlyInComponent* flyIn{ instance->GetComponent<FlyInComponent>() };
		if (flyIn)
		{
			flyIn->spawnDirection = dir;
		}
	}

	EnemyTag* tag{ instance->GetComponent<EnemyTag>() };
	if(tag)
	{
		tag->state = state;
	}

	out = instance->GetId();
	
	return out;
}

void EnemyManager::Hide(SLD::WorldEntity& world, SLD::GameObjectId id)
{
	using namespace SLD;
	
	for (const auto& [type,enemyVector] : m_EnemyInstances)
	{
		const auto findIt = std::find_if(enemyVector.begin(), enemyVector.end(), [&id](const SharedPtr<SLD::GameObject>& instance)
			{
				return id == instance->GetId();
			});

		if(findIt != enemyVector.end())
		{
			Box2DComponent* box2d{ world.GetComponent<Box2DComponent>(id) };
			if(box2d)
			{
				box2d->SetVelocity({ 0.0f,0.0f });
				box2d->SetPosition({ 0.0f,-720.0f });
			}
		}
		
	}
}

SLD::GameObjectId EnemyManager::GiveRandomType(SLD::WorldEntity& world , EnemyType type)
{
	int rand{};
	while(true)
	{
		rand = SLD::Random::RandInt(0, int(m_EnemyInstances[type].size()) - 1);
		EnemyTag* tag{world.GetComponent<EnemyTag>(m_EnemyInstances[type][rand]->GetId())};
		if (tag->state != EnemyStateNums::Died && tag->state != EnemyStateNums::Dive)
			break;
	}
	
	return m_EnemyInstances[type][rand]->GetId();
}

void EnemyManager::Reset()
{
	m_ActiveEnemyCounter.clear();
}

void EnemyManager::InitializeRowCol()
{
	// Setup Row and Col of Enemies
	// Zako row starts : 3,4
	// Goei row starts : 1,2
	// Galagas row starts : 0

	// Zako
	{
		auto& line{ m_EnemyRowColFormation[EnemyType::Zako] };
		const int startRow{ 3 };
		int col{};
		for (size_t i = 0; i < ZakoCntLimit; ++i)
		{
			const int row{ int(i % 2) + startRow };

			if (i != 0 && i % 2 == 0)
			{
				col++;
			}

			line.emplace_back(std::make_pair(row, col));
		}
	}

	// Goei
	{
		auto& line{ m_EnemyRowColFormation[EnemyType::Goei] };
		const int startRow{ 1 };
		int col{};
		for (size_t i = 0; i < GoeiCntLimit; ++i)
		{
			const int row{ int(i % 2) + startRow };
			if (i != 0 && i % 2 == 0)
			{
				col++;
			}

			line.emplace_back(std::make_pair(row, col));
		}
	}

	// Galagas
	{
		auto& line{ m_EnemyRowColFormation[EnemyType::Galagas] };
		const int row{ 0 }; // Only exist in first row
		for (size_t i = 0; i < GalagasCntLimit; ++i)
		{
			line.emplace_back(std::make_pair(row, int(i)));
		}
	}

}

