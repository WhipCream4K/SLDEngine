
#ifndef SLDFRAMEWORK_TRANSFORMCOMPONENT_H
#define SLDFRAMEWORK_TRANSFORMCOMPONENT_H

#include "TickComponent.h"

namespace SLD
{
	
	class TransformComponent : public TickComponent
	{
	public:

		static constexpr const char* UniqueId{ "TransformComponent" };
		
		TransformComponent();

		void Translate(float x, float y, float z);
		void Translate(const rtm::float3f& pos);
		void SetRotation(float x, float y, float z);
		void SetRotation(const rtm::float4f& rot);
		void SetRotation(const rtm::quatf& rot);
		void SetScale(float x, float y, float z);
		void SetScale(const rtm::float3f& scale);

		[[nodiscard]] const rtm::float3f& GetWorldPos() const;
		[[nodiscard]] const rtm::float3f& GetLocalScale() const;
		[[nodiscard]] const rtm::float4f& GetLocalRotationQuat() const;
		[[nodiscard]] rtm::float3f GetLocalRotationEuler() const;
		[[nodiscard]] const rtm::qvvf& GetWorldFinishMatrix() const;
		//[[nodiscard]] const rtm::matrix4x4f& GetWorldMatrix() const;

		void AsyncUpdate(float deltaTime) override;

	private:

		void UpdateWorldMatrix();
		
		rtm::float3f m_WorldPos;
		rtm::float3f m_Scale;
		rtm::float4f m_Rotation;
		//rtm::matrix4x4f m_WorldMatrix;
		rtm::qvvf m_WorldFinishMatrix;
		bool m_HasUpdateWorldMatrix;
	};
}


#endif
