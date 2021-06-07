

#ifndef SLDFRAMEWORK_EVENT_H
#define SLDFRAMEWORK_EVENT_H

#include "Delegate.h"
#include <tuple>

namespace SLD
{
	// Base Interface Class just so that any class can hold this interface and allocates any event during runtime
	struct IEvent
	{
		IEvent() = default;
		virtual void Invoke() const = 0;
		virtual ~IEvent() = default;

		IEvent(const IEvent&) = delete;
		IEvent& operator=(const IEvent&) = delete;
		IEvent(IEvent&&) = delete;
		IEvent& operator=(IEvent&&) = delete;
	};

	// size Event == sizeof(void*) + sizeof(Args...) + sizeof(Delegate)
	template<typename FnType, class Object = Empty>
	class Event {};

	template<typename Ret, class Object, typename ...Args>
	class Event<Ret(Object::*)(Args...)> : public IEvent
	{
		using FnTpye = Ret(Args...);

	public:

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj)
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, const std::tuple<Args...>& tuple)
			: m_Delegate(ptr, usrObj)
			, m_Data(tuple)
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, Object* usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj)
			, m_Data(std::forward_as_tuple(args...))
		{
		}


		void Invoke() const override
		{
			ApplyTuple(std::index_sequence_for<Args...>());
		}

		~Event() override = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

	private:

		template<std::size_t... Is>
		void ApplyTuple(std::index_sequence<Is...>)const
		{
			m_Delegate(std::forward<Args>(std::get<Is>(m_Data))...);
		}

		Delegate<FnTpye, Object> m_Delegate;
		std::tuple<Args...> m_Data;
	};

	template<typename Ret, class Object, typename ...Args>
	class Event<Ret(Object::*)(Args...) const> : public IEvent
	{
		using FnTpye = Ret(Args...) const;

	public:

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj.lock().get())
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, const std::tuple<Args...>& tuple)
			: m_Delegate(ptr, usrObj.lock().get())
			, m_Data(tuple)
		{
		}


		void Invoke() const override
		{
			ApplyTuple(std::index_sequence_for<Args...>());
		}

		~Event() override = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

	private:

		template<std::size_t... Is>
		void ApplyTuple(std::index_sequence<Is...>) const
		{
			m_Delegate(std::forward<Args>(std::get<Is>(m_Data))...);
		}

		Delegate<FnTpye, Object> m_Delegate;
		std::tuple<Args...> m_Data;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	///
	///
	///
	///
	///
	///
	///
	/////////////////////////////////////////////////////////////////////////////////////
	///

	template<typename Ret, class Object, typename ...Args>
	class Event<Ret(Args...), Object> : public IEvent
	{
		using FnTpye = Ret(Args...);

	public:

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj.lock().get())
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, Args&&... args)
			: m_Delegate(ptr)
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, const std::tuple<Args...>& tuple)
			: m_Delegate(ptr, usrObj.lock().get())
			, m_Data(tuple)
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, Object* usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj)
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		void Invoke() const override
		{
			ApplyTuple(std::index_sequence_for<Args...>());
		}

		~Event() override = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

	private:

		template<std::size_t... Is>
		void ApplyTuple(std::index_sequence<Is...>)const
		{
			m_Delegate(std::forward<Args>(std::get<Is>(m_Data))...);
		}

		Delegate<FnTpye, Object> m_Delegate;
		std::tuple<Args...> m_Data;
	};

	template<typename Ret, class Object, typename ...Args>
	class Event<Ret(Args...) const, Object> : public IEvent
	{
		using FnTpye = Ret(Args...) const;

	public:

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj.lock().get())
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, Args&&... args)
			: m_Delegate(ptr)
			, m_Data(std::forward_as_tuple(args...))
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, WeakPtr<Object> usrObj, const std::tuple<Args...>& tuple)
			: m_Delegate(ptr, usrObj.lock().get())
			, m_Data(tuple)
		{
		}

		template<typename FuncPtr>
		Event(FuncPtr ptr, Object* usrObj, Args&&... args)
			: m_Delegate(ptr, usrObj)
			, m_Data(std::forward_as_tuple(args...))
		{
		}
		
		void Invoke() const override
		{
			ApplyTuple(std::index_sequence_for<Args...>());
		}

		~Event() override = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

	private:

		template<std::size_t... Is>
		void ApplyTuple(std::index_sequence<Is...>)const
		{
			m_Delegate(std::forward<Args>(std::get<Is>(m_Data))...);
		}

		Delegate<FnTpye, Object> m_Delegate;
		std::tuple<Args...> m_Data;
	};

	using EventHandler = IEvent;

	template<typename FuncPtr, typename UserClass, typename ...Args>
	RefPtr<EventHandler> CreateEvent(const RefPtr<UserClass>& objInstance, FuncPtr fnPtr, Args&&... args)
	{
		return std::make_shared<Event<FuncPtr, UserClass>>(fnPtr, objInstance, std::forward<Args>(args)...);
	}

}


#endif

