
#ifndef SLDFRAMEWORK_OBSERVEPTR_H
#define SLDFRAMEWORK_OBSERVEPTR_H

//#include "Base.h"
#include <type_traits>

namespace SLD
{
	// A class that watch over some un-owned pointer
	template<typename T>
	class ObservePtr final
	{
	public:

		ObservePtr();
		ObservePtr(std::add_pointer_t<T>* addressValue);
		ObservePtr(std::add_pointer_t<T> ptrValue);
		ObservePtr(const ObservePtr& other);
		ObservePtr& operator=(const ObservePtr& other);
		ObservePtr(ObservePtr&& other) noexcept;
		ObservePtr& operator=(ObservePtr&& other) noexcept;

		ObservePtr& operator=(std::add_pointer_t<T>* addressValue) noexcept;
		ObservePtr& operator=(std::add_pointer_t<T> ptrValue) noexcept;

		~ObservePtr();

		T& GetRef() const noexcept(true)
		{
			return **m_WatchValue;
		}

		operator bool() const
		{
			return m_WatchValue;
		}

		std::add_lvalue_reference_t<T*> GetAddress() const
		{
			return m_WatchValue;
		}

		std::add_lvalue_reference_t<T*> GetPtr() const
		{
			return *m_WatchValue;
		}

		std::add_lvalue_reference_t<T*> operator*() const
		{
			return *m_WatchValue;
		}

		std::add_lvalue_reference_t<T*> operator->() const
		{
			return *m_WatchValue;
		}

	private:

		T** m_WatchValue;
	};

	template <typename T>
	ObservePtr<T>::ObservePtr()
		: m_WatchValue()
	{
	}

	template <typename T>
	ObservePtr<T>::ObservePtr(std::add_pointer_t<T>* addressValue)
		: m_WatchValue(addressValue)
	{
	}


	template <typename T>
	ObservePtr<T>::ObservePtr(std::add_pointer_t<T> ptrValue)
		: m_WatchValue(&ptrValue)
	{

	}

	template <typename T>
	ObservePtr<T>::ObservePtr(const ObservePtr& other)
		: m_WatchValue(other.m_WatchValue)
	{
	}

	template <typename T>
	ObservePtr<T>& ObservePtr<T>::operator=(const ObservePtr& other)
	{
		if (this != &other)
		{
			m_WatchValue = other.m_WatchValue;
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>::ObservePtr(ObservePtr&& other) noexcept
		: m_WatchValue(std::move(other.m_WatchValue))
	{
	}

	template <typename T>
	ObservePtr<T>& ObservePtr<T>::operator=(ObservePtr&& other) noexcept
	{

		if (this != &other)
		{
			m_WatchValue = std::move(other.m_WatchValue);
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>& ObservePtr<T>::operator=(std::add_pointer_t<T>* addressValue) noexcept
	{
		if (m_WatchValue != addressValue)
		{
			m_WatchValue = addressValue;
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>& ObservePtr<T>::operator=(std::add_pointer_t<T> ptrValue) noexcept
	{
		if (m_WatchValue != &ptrValue)
		{
			m_WatchValue = &ptrValue;
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>::~ObservePtr()
	{
	}
}


#endif
