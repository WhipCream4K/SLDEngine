
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

		enum class Type
		{
			TransformComponent,

			Count // Use to keep track of all the tick component
		};

		virtual void AsyncUpdate([[maybe_unused]] float) {}

		virtual ~TickComponent() = default;
		
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
}

#endif
