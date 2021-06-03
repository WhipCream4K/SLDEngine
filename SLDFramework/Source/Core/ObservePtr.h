
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

		// Quality of life
		ObservePtr();
		ObservePtr(std::add_pointer_t<void> const& voidException, size_t offset);
		ObservePtr(std::add_pointer_t<uint8_t> const& ptrValue, size_t offset);
		ObservePtr(const ObservePtr& other);
		ObservePtr& operator=(const ObservePtr& other);
		ObservePtr(ObservePtr&& other) noexcept;
		ObservePtr& operator=(ObservePtr&& other) noexcept;

		~ObservePtr();

		operator bool() const
		{
			return m_BufferStart;
		}

		uint8_t* const& GetHead() const noexcept
		{
			return m_BufferStart;
		}

		size_t GetOffset() const noexcept
		{
			return m_Offset;
		}

		//template<typename T>
		[[nodiscard]] T* GetPtr() const noexcept
		{
			return (m_BufferStart) ? reinterpret_cast<T*>(m_BufferStart + m_Offset) : nullptr;
		}

		[[nodiscard]] uint8_t* GetPtrPointTo() const
		{
			return m_BufferStart + m_Offset;
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

		// NOTE: I could have use list to store a list of pointers and observe pointer would be useless
		// however the list::erase() time complexity is O(n)
		// creating dangling pointer and have smart pointer wraps around observe pointer is better imo.
		uint8_t* const& m_BufferStart;
		size_t m_Offset;
	};

	template <typename T>
	ObservePtr<T>::ObservePtr()
		: m_BufferStart(nullptr)
		, m_Offset()
	{
	}

	template <typename T>
	ObservePtr<T>::ObservePtr(std::add_pointer_t<void>const& voidException, size_t offset)
		: m_BufferStart(reinterpret_cast<uint8_t* const&>(voidException))
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
			const_cast<uint8_t*&>(m_BufferStart) = other.m_BufferStart;
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
			const_cast<uint8_t*&>(m_BufferStart) = std::move(other.m_BufferStart);
			m_Offset = std::move(other.m_Offset);
		}

		return *this;
	}

	template <typename T>
	ObservePtr<T>::~ObservePtr()
	{
		if constexpr (!std::is_null_pointer_v<T>)
			GetPtr()->~T();
	}
}


#endif
