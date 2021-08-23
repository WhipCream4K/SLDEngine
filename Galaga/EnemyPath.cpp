#include "EnemyPath.h"
#include <fstream>
#include <JSON/nlohmann/json.hpp>
#include "FormationWayPoints.h"

EnemyPath::EnemyPath()
	: m_Paths()
	, m_LineFormationWayPoints()
	, m_HopStep()
{
}

void EnemyPath::Initialize()
{
	using namespace SLD;

	const std::string spawnFilePath{ "./Resources/MapFiles/SpawnBezierPoints.json" };
	std::ifstream spawnFile{ spawnFilePath };

	nlohmann::json parser;
	spawnFile >> parser;

	std::array<rtm::float2f, 4> bezierPoint{};

	// parse json to system
	{
		size_t direction{};
		for (auto it = parser.begin(); it != parser.end(); ++it)
		{
			BezierPath path{};
			const auto& name{ it.key() };

			if (name == "BottomLeft")
				direction = size_t(SpawnDirection::BottomLeft);
			else if (name == "TopLeft")
				direction = size_t(SpawnDirection::TopLeft);
			else if (name == "TopRight")
				direction = size_t(SpawnDirection::TopRight);
			else
				direction = size_t(SpawnDirection::BottomRight);

			for (const auto& val : it->items())
			{
				size_t count{};
				for (const auto& points : val.value()["Points"])
				{
					float p[2]{};
					size_t idx{};
					for (const auto& point : points.items())
					{
						p[idx++] = float(point.value());
					}

					bezierPoint[count++] = { p[0],p[1] };
				}

				path.AddCurve(BezierCurve{
					bezierPoint[0],
				bezierPoint[1],
				bezierPoint[2],
				bezierPoint[3] }
				, val.value()["Samples"]);
			}

			if (direction < m_Paths.max_size())
				m_Paths[direction] = path.Sample();
		}
	}


	{
		m_LineFormationWayPoints.resize(5);
		
		m_LineFormationWayPoints[0].resize(4);
		m_LineFormationWayPoints[1].resize(8);
		m_LineFormationWayPoints[2].resize(8);
		m_LineFormationWayPoints[3].resize(10);
		m_LineFormationWayPoints[4].resize(10);

		float yStart{ 300.0f };
		for (auto& line : m_LineFormationWayPoints)
		{
			float xStart{ 25.0f };
			int step{};
			for (size_t i = 0; i < line.size(); ++i)
			{
				if (step == 2)
				{
					step = 0;
					xStart += 50.0f;
				}
				
				line[i].x = xStart;
				line[i].y = yStart;
				xStart *= -1;
				step++;
			}

			yStart -= 50.0f;
		}
		
	}
}

const std::vector<rtm::float2f>& EnemyPath::GetPath(SpawnDirection dir)
{
	return m_Paths[size_t(dir)];
}

const SpawnPaths& EnemyPath::GetAllSpawnPaths() const
{
	return m_Paths;
}

const EnemyPath::LineWayPoints& EnemyPath::GetFormationWayPoints() const
{
	return m_LineFormationWayPoints;
}

void EnemyPath::SetFormationTracker(SLD::GameObjectId id)
{
	m_FormationHolder = id;
}

void EnemyPath::TrackerPathReset(SLD::WorldEntity& world)
{
	auto* formation = world.GetComponent<FormationWayPoints>(m_FormationHolder);
	if(formation)
	{
		formation->GetWayPoints() = GetFormationWayPoints();
	}
}

void EnemyPath::Destroy(SLD::WorldEntity& world)
{
	world.DestroyGameObject(m_FormationHolder);
}

SLD::GameObjectId EnemyPath::GetFormationTracker() const
{
	return m_FormationHolder;
}
