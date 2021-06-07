
#ifndef SLDFRAMEWORK_NONTICKCOMPONENT_H
#define SLDFRAMEWORK_NONTICKCOMPONENT_H

#include "../Core/Base.h"
#include "BaseComponent.h"

namespace SLD
{
	class NonTickComponent : public BaseComponent
	{
	public:

		~NonTickComponent() override = default;
		
		//using value_type = std::remove_all_extents_t<T>;
		//using pointer = value_type*;
		//
		//static void DoCreate(const std::any& instance)
		//{
		//	std::any_cast<pointer>(instance)->OnCreate();
		//}

		//template<typename = std::enable_if_t<std::is_member_function_pointer_v<decltype(T::OnUpdate)>,T>>
		//NonTickComponent()
		//{
		//}
		
		//NonTickComponent()
		//	: m_OnCreate()
		//	, m_Instance()
		//	, m_IsInitialized()
		//{
		//}

		//void operator()()
		//{
		//	if (!m_IsInitialized)
		//	{
		//		m_IsInitialized = true;
		//		m_OnCreate(m_Instance);
		//	}
		//}


		//OnCreateEvent m_OnCreate;
		//std::any m_Instance;
		//bool m_IsInitialized;
	};
}

#endif
