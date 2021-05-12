
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
		void SetRotation(const rtm::float3f& rot);
		void SetScale(float x, float y, float z);
		void SetScale(const rtm::float3f& scale);

		[[nodiscard]] const rtm::matrix4x4f& GetWorldMatrix() const;

	private:

		rtm::float3f m_WorldPos;
		rtm::float3f m_Rotation;
		rtm::float3f m_Scale;
		rtm::matrix4x4f m_WorldMatrix;
		bool m_HasUpdateWorldMatrix;
	};
}


#endif
