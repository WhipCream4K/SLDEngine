
#ifndef SLDFRAMEWORK_NONTICKCOMPONENT_H
#define SLDFRAMEWORK_NONTICKCOMPONENT_H

#include "../Core/Base.h"
#include "BaseComponent.h"

namespace SLD
{
	class NonTickComponent : public BaseComponent
	{
	public:
		
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

	//template <typename UserClass>
	//constexpr auto NonTickComponent::Bind(
	//	const RefPtr<std::remove_all_extents_t<UserClass>>& instance) noexcept(false) -> void
	//{
	//	using value_type = std::remove_all_extents_t<UserClass>;
	//	using pointer = value_type*;
	//	
	//	m_OnCreate = [](const std::any& objInstance)
	//	{
	//		std::any_cast<pointer>(objInstance)->OnCreate();
	//	};

	//	m_Instance = instance.get();
	//}
}

#endif
