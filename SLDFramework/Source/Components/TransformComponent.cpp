#include "TransformComponent.h"
#include "../Math/rtm/matrix4x4f.h"
#include "../Math/rtm/vector4f.h"
#include "../Math/rtm/qvvf.h"

SLD::TransformComponent::TransformComponent()
	: m_WorldPos()
	, m_Scale(rtm::float3f{ 1.0f,1.0f,1.0f })
	, m_Rotation()
	, m_WorldFinishMatrix()
	, m_State(TransformState::ShouldUpdateMatrix)
{
	UpdateWorldMatrix();
}

SLD::TransformComponent::~TransformComponent()
{
}


void SLD::TransformComponent::Translate(float x, float y, float z)
{
	m_WorldPos.x = x;
	m_WorldPos.y = y;
	m_WorldPos.z = z;

	m_WorldFinishMatrix.translation = rtm::vector_load3(&m_WorldPos);
	m_State = TransformState::ShouldUpdateMatrix;
}


void SLD::TransformComponent::Translate(const rtm::float3f& pos)
{
	m_WorldPos = pos;
	m_WorldFinishMatrix.translation = rtm::vector_load3(&m_WorldPos);
	m_State = TransformState::ShouldUpdateMatrix;
}

void SLD::TransformComponent::Translate(const rtm::vector4f& pos)
{
	rtm::float3f newPos{};
	rtm::vector_store3(pos, &newPos);
	m_WorldPos = newPos;
	m_WorldFinishMatrix.translation = pos;
	m_State = TransformState::ShouldUpdateMatrix;
}

void SLD::TransformComponent::SetRotation(float x, float y, float z)
{
	const auto rotation = rtm::quat_from_euler(x, y, z);
	rtm::quat_store(rotation, &m_Rotation);

	m_WorldFinishMatrix.rotation = rtm::quat_load(&m_Rotation);
	m_State = TransformState::ShouldUpdateMatrix;
}

void SLD::TransformComponent::SetRotation(const rtm::float4f& rot)
{
	m_Rotation = rot;

	m_WorldFinishMatrix.rotation = rtm::quat_load(&m_Rotation);

	m_State = TransformState::ShouldUpdateMatrix;
}

void SLD::TransformComponent::SetRotation(const rtm::quatf& rot)
{
	rtm::quat_store(rot, &m_Rotation);

	m_WorldFinishMatrix.rotation = rtm::quat_load(&m_Rotation);
	m_State = TransformState::ShouldUpdateMatrix;
}

void SLD::TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;

	m_WorldFinishMatrix.scale = rtm::vector_load3(&m_Scale);
	m_State = TransformState::ShouldUpdateMatrix;
}

void SLD::TransformComponent::SetScale(const rtm::float3f& scale)
{
	m_Scale = scale;
	m_WorldFinishMatrix.scale = rtm::vector_load3(&m_Scale);

	m_State = TransformState::ShouldUpdateMatrix;
}

const rtm::float3f& SLD::TransformComponent::GetWorldPos() const
{
	return m_WorldPos;
}

const rtm::float3f& SLD::TransformComponent::GetLocalScale() const
{
	return m_Scale;
}

const rtm::float4f& SLD::TransformComponent::GetLocalRotationQuat() const
{
	return m_Rotation;
}

rtm::float3f SLD::TransformComponent::GetLocalRotationEuler() const
{
	return rtm::float3f{};
}

SLD::TransformState SLD::TransformComponent::GetTransformState() const
{
	return m_State;
}

const rtm::qvvf& SLD::TransformComponent::GetWorldMatrix() const
{
	return m_WorldFinishMatrix;
}

const rtm::qvvf& SLD::TransformComponent::GetWorldMatrix()
{
	switch (m_State)
	{
	case TransformState::ShouldUpdateMatrix:	UpdateWorldMatrix(); return m_WorldFinishMatrix;
	case TransformState::UpToDateMatrix:		return m_WorldFinishMatrix;
	}

	return m_WorldFinishMatrix;
}

void SLD::TransformComponent::UpdateWorldMatrix()
{
	// scale
	m_WorldFinishMatrix.scale = rtm::vector_load3(&m_Scale);

	// rotation
	m_WorldFinishMatrix.rotation = rtm::quat_load(&m_Rotation);

	// translation
	m_WorldFinishMatrix.translation = rtm::vector_load3(&m_WorldPos);

	m_State = TransformState::UpToDateMatrix;
}
