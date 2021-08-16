#pragma once

#include <Core/Singleton.h>
#include "MyComponents.h"
#include "SpawnData.h"


class SpawnStages : public SLD::Singleton<SpawnStages>
{
public:

	friend class SLD::Singleton<SpawnStages>;

	void Initialize();
	[[nodiscard]] const std::vector<SpawnData>& GetSpawnData() const;
	[[nodiscard]] const StageSpawnData& GetSpawnStage() const;
	
private:

	// Stage( Wave type  + interval )
	std::vector<SpawnData> m_SpawnData;
	std::vector<std::vector<SpawnData>> m_SpawnStages;
};

