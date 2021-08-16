
#ifndef SLDFRAMEWORK_TRANSFORMCOMPONENT_H
#define SLDFRAMEWORK_TRANSFORMCOMPONENT_H

#include "TickComponent.h"
#include <optional>

namespace SLD
{
	enum class TransformState
	{
		ShouldUpdateMatrix,
		UpToDateMatrix
	};
	
	class TransformComponent final : public ComponentT<TransformComponent>
	{
	public:
	
		static constexpr const char* UniqueId{ "TransformComponent" };
		
		TransformComponent();
		~TransformComponent();
		void Translate(float x, float y, float z);
		void Translate(const rtm::float3f& pos);
		void Translate(const rtm::vector4f& pos);
		void SetRotation(float x, float y, float z);
		void SetRotation(const rtm::float4f& rot);
		void SetRotation(const rtm::quatf& rot);
		void SetScale(float x, float y, float z);
		void SetScale(const rtm::float3f& scale);
		
		[[nodiscard]] const rtm::float3f& GetWorldPos() const;
		[[nodiscard]] const rtm::float3f& GetLocalScale() const;
		[[nodiscard]] const rtm::float4f& GetLocalRotationQuat() const;
		[[nodiscard]] rtm::float3f GetLocalRotationEuler() const;

		TransformState GetTransformState() const;
		const rtm::qvvf& GetWorldMatrix() const;
		const rtm::qvvf& GetWorldMatrix();

	private:

		void UpdateWorldMatrix();
		
		rtm::float3f m_WorldPos;
		rtm::float3f m_Scale;
		rtm::float4f m_Rotation;
		rtm::qvvf m_WorldFinishMatrix;
		TransformState m_State;
	};
}


#endif
