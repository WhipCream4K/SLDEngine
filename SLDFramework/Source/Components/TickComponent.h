
#ifndef SLDFRAMEWORK_TICKCOMPONENT_H
#define SLDFRAMEWORK_TICKCOMPONENT_H

#include "../Core/Base.h"
#include "BaseComponent.h"
#include "../Core/ScriptableChecks.h"


namespace SLD
{
	class TickComponent : public BaseComponent
	{
	public:
		//using pointer = typename NonTickComponent<T>::pointer;
		////using OnCreateEvent = void(*)(const std::any&);
		////using OnUpdateEvent = void(*)(const std::any&, float);

		//template<typename = std::enable_if_t<std::is_member_function_pointer_v<decltype(T::OnUpdate)>,T>>
		//TickComponent()
		//{
		//}

		//template<typename UserClass>
		//constexpr auto Bind(const RefPtr<UserClass>& instance) noexcept(false) -> void;

		//void operator()(float dt) const
		//{
		//	m_OnUpdate(m_Instance, dt);
		//}

		// assume this any is pointer
		//static void DoTick(const std::any& instance,float dt)
		//{
		//	std::any_cast<pointer>(instance)->OnUpdate(dt);
		//}

		//OnCreateEvent m_OnCreate;
		//OnUpdateEvent m_OnUpdate;
		//std::any m_Instance;

	};

	//template <typename UserClass>
	//constexpr auto TickComponent::Bind(
	//	const RefPtr<UserClass>& instance) noexcept(false) -> void
	//{
	//	using value_type = std::remove_all_extents_t<UserClass>;
	//	using pointer = value_type*;
	//	
	//	m_OnUpdate = [](const std::any& objInstance, float dt)
	//	{
	//		std::any_cast<pointer>(objInstance)->OnUpdate(dt);
	//	};

	//	m_OnCreate = [](const std::any& objInstance)
	//	{
	//		std::any_cast<pointer>(objInstance)->OnCreate();
	//	};

	//	static_assert(IsFuncSameType<decltype(&value_type::OnUpdate),void,float>,
	//		"OnUpdate Function is not the same type as void()(float)");

	//	m_Instance = instance.get();
	//}
}

#endif
