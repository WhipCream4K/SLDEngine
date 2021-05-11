#include "TransformComponent.h"

SLD::TransformComponent::TransformComponent()
	: TickComponent()
	, m_WorldPos()
	, m_Rotation()
	, m_Scale()
	, m_WorldMatrix()
	, m_HasUpdateWorldMatrix()
{
}


void SLD::TransformComponent::Translate(float x, float y, float z)
{
	m_WorldPos.x = x;
	m_WorldPos.y = y;
	m_WorldPos.z = z;
}


void SLD::TransformComponent::Translate(const rtm::float3f& pos)
{
	m_WorldPos = pos;
}

void SLD::TransformComponent::SetRotation(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void SLD::TransformComponent::SetRotation(const rtm::float3f& rot)
{
	m_Rotation = rot;
}

void SLD::TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void SLD::TransformComponent::SetScale(const rtm::float3f& scale)
{
	m_Scale = scale;
}

const rtm::matrix4x4f& SLD::TransformComponent::GetWorldMatrix() const
{
	return m_WorldMatrix;
}
