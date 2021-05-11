
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

		friend RefPtr<T> Instance();

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	
	protected:

		static RefPtr<value_type> GetInstance()
		{
			if (!m_Instance)
				m_Instance = std::make_shared<value_type>();
			return m_Instance;
		}
		
		Singleton() = default;
		static RefPtr<value_type> m_Instance;
	};

	template<typename T>
	inline RefPtr<T> Instance()
	{
		return T::GetInstance();
	}
}

#endif

