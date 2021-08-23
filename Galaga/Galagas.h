#pragma once

#include <SLDFramework.h>

#include "MyComponents.h"

class Galagas : public SLD::Prefab
{
public:

	inline static constexpr SLD::Vector4<uint32_t> GalagasRect{ 127,37,16,16 };

	Galagas(SpawnDirection dir, int startRow, int startCol);
	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;
	const SharedPtr<SLD::GameObject>& GetGameObject() const;

private:

	SharedPtr<SLD::GameObject> m_Instance;
	SpawnDirection m_SpawnDirection;
	int m_Col;
	int m_Row;
};

