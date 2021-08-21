#pragma once

#include <functional>

namespace SLD
{
	class Listener
	{
	public:

		virtual ~Listener() = default;
	};

	template<typename ...Args>
	class ListenerT : public Listener
	{
	public:
		
		virtual ~ListenerT() override = default;
		void BroadCast(Args... args);
		void Bind(const std::function<void(Args...)>&f);

	private:

		std::function<void(Args...)> m_Callback;
	};

	template <typename ... Args>
	void ListenerT<Args...>::BroadCast(Args... args)
	{
		m_Callback(std::forward<Args>(args)...);
	}

	template <typename ... Args>
	void ListenerT<Args...>::Bind(const std::function<void(Args...)>& f)
	{
		m_Callback = f;
	}
}


