#pragma once

#include <SLDFramework.h>

class Bullet : public SLD::Prefab
{
public:
	
	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;

	inline static constexpr SLD::Vector4<uint32_t> BulletRect{307,136,16,16};

};

