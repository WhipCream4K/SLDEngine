#pragma once

#include <SLDFramework.h>

class Galagas : public SLD::Prefab
{
public:

	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;
	const SharedPtr<SLD::GameObject>& GetGameObject() const;

private:

	SharedPtr<SLD::GameObject> m_Instance;
};

