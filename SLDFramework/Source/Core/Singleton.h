
#ifndef SLDFRAMEWORK_SINGLETON_H
#define SLDFRAMEWORK_SINGLETON_H

#include "Base.h"

namespace SLD
{

	template<typename T>
	class Singleton;
	
	template<typename T>
	RefPtr<T> Instance();
	
	template<typename T>
	class Singleton
	{
	public:

		using value_type = std::remove_all_extents_t<T>;

		template<typename U>
		friend RefPtr<U> Instance();

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		virtual ~Singleton() = default;

	protected:
		
		static RefPtr<value_type> GetInstance()
		{
			if (!m_Instance)
				m_Instance = std::make_shared<value_type>();
			return m_Instance;
		}
		
		Singleton() = default;

	private:
		
		static RefPtr<value_type> m_Instance;
	};

	template<typename T>
	std::shared_ptr<typename Singleton<T>::value_type> Singleton<T>::m_Instance{};
	
	template<typename T>
	inline RefPtr<T> Instance()
	{
		return Singleton<T>::GetInstance();
	}
}

#endif

