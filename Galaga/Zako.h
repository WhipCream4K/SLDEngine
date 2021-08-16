#pragma once

#include <SLDFramework.h>

enum class SpawnDirection;

class Zako : public SLD::Prefab
{
public:

	Zako(SpawnDirection direction);
	Zako(SpawnDirection dir, int startRow, int startCol);
	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;

	inline static constexpr SLD::Vector4<uint32_t> ZakoRect{127,91,16,16};

	const SharedPtr<SLD::GameObject>& GetGameObjectInstance() const;

private:

	SharedPtr<SLD::GameObject> m_Instance;
	SpawnDirection m_SpawnDirection;
	int m_Col;
	int m_Row;
};

