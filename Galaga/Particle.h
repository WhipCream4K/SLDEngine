#pragma once

#include <SLDFramework.h>

class Particle : public SLD::Prefab
{
public:

	Particle(const SLD::Vector4<uint32_t>& rect);
	void OnCreate(const SharedPtr<SLD::GameObject>& gameObject) override;

private:

	SLD::Vector4<uint32_t> m_SpriteRect;
};

