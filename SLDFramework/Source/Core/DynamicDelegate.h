
#ifndef SLDFRAMEWORK_ACTION_H
#define SLDFRAMEWORK_ACTION_H

#include "Delegate.h"

namespace SLD
{

	//TODO: Name this DynamicDelegate
	template<typename FnType>
	struct IAction{};

	template<typename Ret,typename ...Args>
	struct IAction<Ret(Args...)>
	{
		IAction() = default;
		virtual ~IAction() = default;

		virtual Ret Invoke(Args...) const = 0;
		
		IAction(const IAction&) = delete;
		IAction(IAction&&) = delete;
		IAction& operator=(const IAction&) = delete;
		IAction& operator=(IAction&&) = delete;
	};
	
	template<typename FnType, class Object = Empty>
	class CAction {};

	template<typename Ret, typename ...Args>
	class CAction<Ret(Args...)> : public IAction<Ret(Args...)>
	{
		using FnType = Ret(Args...);

	public:

		template<typename FuncPtr>
		CAction(FuncPtr ptr)
			: m_Delegate(ptr)
		{
		}

		[[maybe_unused]] Ret Invoke(Args... args) const override
		{
			return m_Delegate(std::forward<Args>(args)...);
		}

		~CAction() override = default;
		CAction(const CAction&) = delete;
		CAction(CAction&&) = delete;
		CAction& operator=(const CAction&) = delete;
		CAction& operator=(CAction&&) = delete;

	private:

		Delegate<FnType> m_Delegate;
	};
	
	template<typename Ret, class Object, typename ...Args>
	class CAction<Ret(Args...), Object> : public IAction<Ret(Args...)>
	{
		using FnType = Ret(Object::*)(Args...);

	public:
		
		template<typename FuncPtr>
		CAction(FuncPtr ptr, WeakPtr<Object> usrObj)
			: m_Delegate(ptr, usrObj.lock().get())
		{
		}

		template<typename FuncPtr>
		CAction(FuncPtr ptr, Object* usrObj)
			: m_Delegate(ptr,usrObj)
		{
		}
		
		template<typename FuncPtr>
		CAction(FuncPtr ptr)
			: m_Delegate(ptr)
		{
		}

		[[maybe_unused]] Ret Invoke(Args... args) const override
		{
			return m_Delegate(std::forward<Args>(args)...);
		}

		~CAction() override = default;
		CAction(const CAction&) = delete;
		CAction(CAction&&) = delete;
		CAction& operator=(const CAction&) = delete;
		CAction& operator=(CAction&&) = delete;

	private:

		Delegate<FnType> m_Delegate;
	};

	template<typename Ret, class Object, typename ...Args>
	class CAction<Ret(Object::*)(Args...), Object> : public IAction<Ret(Args...)>
	{
		using FnTpye = Ret(Args...);

	public:

		template<typename FuncPtr>
		CAction(FuncPtr ptr, WeakPtr<Object> usrObj)
			: m_Delegate(ptr, usrObj.lock().get())
		{
		}

		template<typename FuncPtr>
		CAction(FuncPtr ptr, Object* usrObj)
			: m_Delegate(ptr, usrObj)
		{
		}

		template<typename FuncPtr>
		CAction(FuncPtr ptr)
			: m_Delegate(ptr)
		{
		}

		[[maybe_unused]] Ret Invoke(Args... args) const override
		{
			return m_Delegate(std::forward<Args>(args)...);
		}

		~CAction() override = default;
		CAction(const CAction&) = delete;
		CAction(CAction&&) = delete;
		CAction& operator=(const CAction&) = delete;
		CAction& operator=(CAction&&) = delete;

	private:

		Delegate<FnTpye> m_Delegate;
	};
	
	template<typename Fntype>
	using DynamicDelegate = IAction<Fntype>;

	//template<typename FnType, class Object = Empty>
	//struct CallAction {};

	//template<typename Ret, class Object, typename ...Args>
	//struct CallAction<Ret(Args...), Object>
	//{
	//	static [[maybe_unused]] Ret Invoke(const SharedPtr<DynamicDelegate>& instance,Args... args)
	//	{
	//		return std::static_pointer_cast<CAction<Ret(Args...), Object>>(instance)->Invoke(std::forward<Args>(args)...);
	//	}

	//	static [[maybe_unused]] Ret Invoke(const DynamicDelegate& ref,Args... args)
	//	{
	//		return static_cast<CAction<Ret(Args...), Object>>(ref).Invoke(std::forward<Args>(args...));
	//	}
	//};
	
}

#endif