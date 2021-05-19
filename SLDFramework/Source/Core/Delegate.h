

#ifndef SLDFRAMEWORK_DELEGATE_H
#define SLDFRAMEWORK_DELEGATE_H

#include "CustomTypes.h"

namespace SLD
{
	// Just realized there's a type traits check for member function pointer
	template<typename T>
	constexpr bool IsMemberFunc{ std::is_member_function_pointer_v<T> };
	template<typename T>
	// wouldn't work with lambda
	constexpr bool IsStaticFunc{ std::is_function_v<std::remove_pointer_t<T>> };
	template<typename T>
	// although it's a little bit ambiguous but it works
	constexpr bool IsWeakRef{ std::is_same_v<std::weak_ptr<typename T::element_type>,T> };

	template<typename Ret, typename Fn, typename ...Args>
	constexpr bool IsInvokable{ std::is_invocable_r_v<Ret,Fn,Args...> };

	using Empty = std::nullptr_t;

	template<typename FnType, class Object = Empty>
	class Delegate
	{};

	template<typename Ret, class Object, typename ...Args>
	class Delegate<Ret(Args...), Object>
	{
	public:

		// TODO: probably do move constructor and move assignment
		using FnType = Ret(Object::*)(Args...);

		using CallbackType = Ret(*)(FnType, WeakPtr<Object>, Args&&...);

		template<
			typename FuncPtr,
			typename = std::enable_if_t<IsInvokable<Ret, FuncPtr, Object, Args...>>
		>
			explicit Delegate(FuncPtr ptr, Object* pObject)
			: m_pFn(ptr)
			, m_pInstance(pObject)
		{
			//static_assert(std::is_invocable_r_v<Ret, MemFuncPtr, Object, Args...>,
			//	"This function can't be invoke from specified class");

			// placement new; doesn't allocate just construct an object on given memory block
			//m_pCallback = [](FnType ptr, WeakPtr<Object> instance, Args&&... args)
			//{
			//	return std::invoke(ptr, instance.lock(), std::forward<Args>(args)...);
			//};
		}

		template<
			typename FuncPtr,
			typename = std::enable_if_t<IsInvokable<Ret, FuncPtr, Object, Args...>>
		>
			explicit Delegate(FuncPtr ptr, const RefPtr<Object>& pObject)
			: m_pFn(ptr)
			, m_pInstance(pObject.get())
		{
		}

		[[maybe_unused]] Ret operator()(Args... args) const
		{
			//if (m_pCallback && m_pInstance.lock())
			//	return (m_pInstance->*m_pFn)(std::forward<Args>(args)...);
			if (m_pInstance)
				return (m_pInstance->*m_pFn)(std::forward<Args>(args)...);

			// TODO: This is runtime error and probably should be treat in logging system rather than throw
			throw std::runtime_error("Callee is no longer exist and can't be reference");
		}

	private:

		//CallbackType m_pCallback;

		// To use smart pointer we have to specified custom deleter which do nothing
		//std::unique_ptr<CallbackType, std::function<void(CallbackStorageType*)>> m_pCallback;

		FnType m_pFn;
		Object* m_pInstance;
	};

	template<typename Ret, class Object, typename ...Args>
	class Delegate<Ret(Args...) const, Object>
	{
	public:

		// TODO: probably do move constructor and move assignment

		using FnType = Ret(Object::*)(Args...) const;

		using CallbackType = Ret(*)(FnType, WeakPtr<Object>, Args&&...);

		template<
			typename FuncPtr,
			typename = std::enable_if_t<IsInvokable<Ret, FuncPtr, Object, Args...>>
		>
			explicit Delegate(FuncPtr ptr, Object* pObject)
			: m_pFn(ptr)
			, m_pInstance(pObject)
		{
			//static_assert(std::is_invocable_r_v<Ret, MemFuncPtr, Object, Args...>,
			//	"This function can't be invoke from specified class");

			// placement new; doesn't allocate just construct an object on given memory block
			//m_pCallback = [](FnType ptr, WeakPtr<Object> instance, Args&&... args)
			//{
			//	return std::invoke(ptr, instance.lock(), std::forward<Args>(args)...);
			//};
		}

		template<
			typename FuncPtr,
			typename = std::enable_if_t<IsInvokable<Ret, FuncPtr, Object, Args...>>
		>
			explicit Delegate(FuncPtr ptr, const RefPtr<Object>& pObject)
			: m_pFn(ptr)
			, m_pInstance(pObject.get())
		{
		}

		[[maybe_unused]] Ret operator()(Args... args) const
		{
			//if (m_pCallback && m_pInstance.lock())
			//	return (*m_pCallback)(m_pFn, m_pInstance, std::forward<Args>(args)...);
			if (m_pInstance)
				return (m_pInstance->*m_pFn)(std::forward<Args>(args)...);

			// TODO: This is runtime error and probably should be treat in logging system rather than throw
			throw std::runtime_error("Callee is no longer exist and can't be reference");
		}

	private:

		//CallbackType m_pCallback;

		// To use smart pointer we have to specified custom deleter which do nothing
		//std::unique_ptr<CallbackType, std::function<void(CallbackStorageType*)>> m_pCallback;

		FnType m_pFn;
		Object* m_pInstance;
	};

	// static Delegate member size, this is always valid because it always stays the same size
	constexpr size_t DelegateMemberSize{
		sizeof(void*) + // function pointer
		sizeof(void*) * 2	// weak pointer instance
	};

	template<typename Ret, typename ...Args>
	class Delegate<Ret(Args...)>
	{
	public:

		// TODO: probably do move constructor and move assignment

		using FnType = Ret(*)(Args...);
		using CallbackType = Ret(*)(FnType, Args&&...);

		Delegate() = default;

		template<
			typename FuncPtr,
			typename = std::enable_if_t<std::is_invocable_r_v<Ret, FuncPtr, Args...>>
		>
			explicit Delegate(FuncPtr ptr)
			: m_pCallback()
			, m_pFn(ptr)
		{
			m_pCallback = [](FnType ptr, Args&&... args)
			{
				return std::invoke(ptr, std::forward<Args>(args)...);
			};
		}

		[[maybe_unused]] Ret operator()(Args... args) const
		{
			if (m_pCallback)
				return (*m_pCallback)(m_pFn, std::forward<Args>(args)...);

			// TODO: This is runtime error and probably should be treat in logging system rather than throw
			throw std::runtime_error("Callback is no longer exist and can't be reference");
		}

		Delegate<Ret(Args...)>& operator=(FnType fn)
		{
			if (this->m_pFn != fn)
			{
				this->m_pFn = fn;
				this->m_pCallback = [](FnType ptr, Args&&... args)
				{
					return std::invoke(ptr, std::forward<Args>(args)...);
				};
			}

			return *this;
		}

	private:

		CallbackType m_pCallback;
		FnType m_pFn;
	};
}

#endif


