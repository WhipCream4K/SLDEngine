#include "TransformComponent.h"
#include "../Math/rtm/matrix4x4f.h"
#include "../Math/rtm/vector4f.h"
#include "../Math/rtm/qvvf.h"

SLD::TransformComponent::TransformComponent()
	: TickComponent()
	, m_WorldPos()
	, m_Scale(rtm::float3f{ 1.0f,1.0f,1.0f })
	, m_Rotation()
	, m_WorldMatrix()
	, m_WorldFinishMatrix()
	, m_HasUpdateWorldMatrix()
{
	UpdateWorldMatrix();
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
	const auto rotation = rtm::quat_from_euler(x, y, z);
	rtm::quat_store(rotation, &m_Rotation);
}

void SLD::TransformComponent::SetRotation(const rtm::float4f& rot)
{
	m_Rotation = rot;
}

void SLD::TransformComponent::SetRotation(const rtm::quatf& rot)
{
	rtm::quat_store(rot, &m_Rotation);
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

const rtm::qvvf& SLD::TransformComponent::GetWorldFinishMatrix() const
{
	return m_WorldFinishMatrix;
}

const rtm::matrix4x4f& SLD::TransformComponent::GetWorldMatrix() const
{
	return m_WorldMatrix;
}

void SLD::TransformComponent::UpdateWorldMatrix()
{
	// scale
	m_WorldFinishMatrix.scale = rtm::vector_load3(&m_Scale);

	// rotation
	m_WorldFinishMatrix.rotation = rtm::quat_load(&m_Rotation);

	// translation
	m_WorldFinishMatrix.translation = rtm::vector_load3(&m_WorldPos);

	//m_WorldMatrix = rtm::matrix_from_qvv()
}
