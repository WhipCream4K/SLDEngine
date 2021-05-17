
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

		ObservePtr(std::add_pointer_t<void> const& voidException, size_t offset);
		ObservePtr(std::add_pointer_t<uint8_t> const& ptrValue, size_t offset);
		ObservePtr(const ObservePtr& other);
		ObservePtr& operator=(const ObservePtr& other);
		ObservePtr(ObservePtr&& other) noexcept;
		ObservePtr& operator=(ObservePtr&& other) noexcept;

		//ObservePtr& operator=(std::add_pointer_t<T>& ptrValue) noexcept;

		~ObservePtr();

		operator bool() const
		{
			return m_BufferStart;
		}

		T* GetPtr() const noexcept
		{
			return (m_BufferStart) ? reinterpret_cast<T*>(m_BufferStart + m_Offset) : nullptr;
		}

		T* operator*() const noexcept
		{
			return (m_BufferStart) ? reinterpret_cast<T*>(m_BufferStart + m_Offset) : nullptr;
		}

		T* operator->() const noexcept
		{
			return (m_BufferStart) ? reinterpret_cast<T*>(m_BufferStart + m_Offset) : nullptr;
		}

	private:

		uint8_t* const& m_BufferStart;
		size_t m_Offset;
	};

	template <typename T>
	ObservePtr<T>::ObservePtr(std::add_pointer_t<void>const& voidException, size_t offset)
		: m_BufferStart((uint8_t*const&)voidException)
		, m_Offset(offset)
	{
	}

	template <typename T>
	ObservePtr<T>::ObservePtr(std::add_pointer_t<uint8_t>const& ptrValue, size_t offset)
		: m_BufferStart(ptrValue)
		, m_Offset(offset)
	{
	}

	template <typename T>
	ObservePtr<T>::ObservePtr(const ObservePtr& other)
		: m_BufferStart(other.m_BufferStart)
		, m_Offset(other.m_Offset)
	{
	}

	template <typename T>
	ObservePtr<T>& ObservePtr<T>::operator=(const ObservePtr& other)
	{
		if (this != &other)
		{
			m_BufferStart = other.m_BufferStart;
			m_Offset = other.m_Offset;
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>::ObservePtr(ObservePtr&& other) noexcept
		: m_BufferStart(std::move(other.m_BufferStart))
		, m_Offset(std::move(other.m_Offset))
	{
	}

	template <typename T>
	ObservePtr<T>& ObservePtr<T>::operator=(ObservePtr&& other) noexcept
	{
		if (this != &other)
		{
			m_BufferStart = std::move(other.m_BufferStart);
			m_Offset = std::move(other.m_Offset);
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>::~ObservePtr()
	{
	}
}


#endif
