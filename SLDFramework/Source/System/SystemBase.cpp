#include "SystemBase.h"
#include "../Core/WorldEntity.h"

SLD::SystemBase::SystemBase(WorldEntity& world, PipelineLayer layer)
	: m_Layer(layer)
	, m_World(world)
{
}

const std::string& SLD::SystemBase::GetKey() const
{
	return m_Key;
}

SLD::PipelineLayer SLD::SystemBase::GetLayer() const
{
	return m_Layer;
}
