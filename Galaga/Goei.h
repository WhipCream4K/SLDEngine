#pragma once

#include <SLDFramework.h>

#include "MyComponents.h"

class Goei : public SLD::Prefab
{
public:

	Goei(SpawnDirection dir);
	Goei(SpawnDirection dir,int startRow,int startCol);

	inline static constexpr SLD::Vector4<uint32_t> GoeiRect{ 109,73,16,16 };

	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;
	const SharedPtr<SLD::GameObject>& GetGameObject() const;

private:

	SharedPtr<SLD::GameObject> m_Instance;
	SpawnDirection m_SpawnDirection;
	int m_Col;
	int m_Row;
};

