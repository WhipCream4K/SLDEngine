#include "SpawnStages.h"
#include <JSON/nlohmann/json.hpp>
#include <fstream>

void SpawnStages::Initialize()
{
	const std::string spawnDataPath{ "./Resources/Mapfiles/EnemySpawnStage-1.json" };
	std::ifstream spawnDataFile{ spawnDataPath };

	nlohmann::json parser{};
	spawnDataFile >> parser;

	// Parse data (ugly)
	{
		for (const auto& wave : parser.items())
		{
			//std::cout << wave.value() << '\n';
			std::vector<SpawnData> spawnSection{};
			for (const auto& item : wave.value().items())
			{
				SpawnData data{};
				for (const auto& [key,value]: item.value()["Section"].items())
				{
					SpawnDirection dir{ value["Direction"] };
					std::vector<EnemyType> enemyTypes{};

					for (const auto& type : value["Types"])
					{
						if (type == "Bee")
							enemyTypes.push_back(EnemyType::Zako);
						else if (type == "Butterfly")
							enemyTypes.push_back(EnemyType::Goei);
						else
							enemyTypes.push_back(EnemyType::Galagas);
					}

					data.wave.try_emplace(dir, enemyTypes);
					//data.section.emplace_back()
				}

				data.interval = item.value()["Interval"];
				//m_SpawnData.emplace_back(data);
				spawnSection.emplace_back(data);
			}
			m_SpawnStages.push_back(spawnSection);
		}
	}
}

const std::vector<SpawnData>& SpawnStages::GetSpawnData() const
{
	return m_SpawnData;
}

const StageSpawnData& SpawnStages::GetSpawnStage() const
{
	return m_SpawnStages;
}
