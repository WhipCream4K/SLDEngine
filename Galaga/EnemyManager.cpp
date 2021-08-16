#include "EnemyManager.h"
#include <Helpers/utils.h>
#include "EnemyVariable.h"
#include "Zako.h"
#include "Goei.h"
#include "Galagas.h"

EnemyManager::EnemyManager()
	: m_EnemyRowColFormation()
	, m_EnemyInstances()
{
	m_MaxEnemyCounter[EnemyType::Zako] = ZakoCntLimit;
	m_MaxEnemyCounter[EnemyType::Goei] = GoeiCntLimit;
	m_MaxEnemyCounter[EnemyType::Galagas] = GalagasCntLimit;

	InitializeRowCol();
}

void EnemyManager::Spawn(SLD::WorldEntity& world, const rtm::float3f& pos, EnemyType type, SpawnDirection dir)
{
	using namespace SLD;

	size_t& activeEnemyCnt{ m_ActiveEnemyCounter[type] };
	const size_t maxEnemyCnt{ m_MaxEnemyCounter[type] };
	const auto& [row, col] = m_EnemyRowColFormation[type][activeEnemyCnt];

	if (activeEnemyCnt < maxEnemyCnt)
	{
		switch (type)
		{
		case EnemyType::Zako:
		{
			SharedPtr<Zako> zako{};
			InstantiatePrefab<Zako>(world, { dir,row,col }, pos, zako);

			m_EnemyInstances[type].emplace_back(zako->GetGameObjectInstance());

		}
		break;
		case EnemyType::Goei:
		{
			SharedPtr<Goei> goei{};
			InstantiatePrefab<Goei>(world, { dir,row,col }, pos, goei);

			m_EnemyInstances[type].emplace_back(goei->GetGameObject());

		}
		break;
		case EnemyType::Galagas:
		{
			SharedPtr<Goei> goei{};
			InstantiatePrefab<Goei>(world, { dir,row,col }, pos, goei);

			m_EnemyInstances[type].emplace_back(goei->GetGameObject());
		}

		break;
		default: break;
		}

		activeEnemyCnt++;
	}
	else
	{
		SharedPtr<GameObject> instance{ m_EnemyInstances[type][activeEnemyCnt % maxEnemyCnt] };
		
	}

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

