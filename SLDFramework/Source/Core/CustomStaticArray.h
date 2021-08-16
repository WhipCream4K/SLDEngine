

#ifndef SLDFRAMEWORK_CUSTOM_STATIC_ARRAY_H
#define SLDFRAMEWORK_CUSTOM_STATIC_ARRAY_H

#include "Base.h"
#include <queue>
#include <array>
#include <optional>

namespace SLD
{
	/// <summary>
	/// Note: This class serves as a handle for stack memory
	/// and is not an owner of any type
	/// </summary>
	/// <typeparam name="T">Type of the element</typeparam>
	template<typename T>
	class CustomStaticArray
	{
	public:

		using value_type = std::remove_all_extents_t<T>;
		using reference = value_type&;
		using pointer = value_type*;

#pragma region Iterator
		class Iterator : public std::random_access_iterator_tag
		{
			friend class CustomStaticArray;
		public:
			Iterator(pointer pointerTo)
				: m_Ptr(pointerTo)
			{
			}

			~Iterator()
			{
				m_Ptr = nullptr;
			}

			Iterator(const Iterator& other)
				: m_Ptr(other.m_Ptr)
			{
			}

			Iterator(Iterator&& other) noexcept
				: m_Ptr(std::move(other.m_Ptr))
			{
			}

			Iterator& operator=(Iterator&& other) noexcept
			{
				if (this != &other)
				{
					m_Ptr = std::move(other.m_Ptr);
				}

				return *this;
			}

			Iterator& operator=(const Iterator& other)
			{
				if (this != &other)
				{
					m_Ptr = other.m_Ptr;
				}

				return *this;
			}

			// dereference operator
			reference operator*()
			{
				return *m_Ptr;
			}

			pointer operator->()
			{
				return m_Ptr;
			}

			Iterator operator++(int)
			{
				Iterator temp = *this;
				++* this;
				return temp;
			}
			Iterator& operator++()
			{
				++m_Ptr;
				return *this;
			}
			Iterator operator--(int)
			{
				Iterator temp = *this;
				--* this;
				return temp;
			}
			Iterator& operator--()
			{
				--m_Ptr;
				return *this;
			}
			bool operator!=(const Iterator& other) const
			{
				return m_Ptr != other.m_Ptr;
			}
			bool operator==(const Iterator& other) const
			{
				return m_Ptr == other.m_Ptr;
			}

		private:

			pointer m_Ptr;
		};
#pragma endregion

#pragma region Const_Iterator
		// TODO: Finish const iterator
		class Const_Iterator : public std::random_access_iterator_tag
		{
		public:
			Const_Iterator(const CustomStaticArray& ref, size_t pos)
				: m_Ref(ref)
				, m_Pos(pos)
			{
			}

			const CustomStaticArray& operator*() const
			{
				return m_Ref;
			}

			void operator++() { ++m_Pos; }
			bool operator!=(size_t pos) const
			{
				return pos != m_Pos;
			}

		private:
			const CustomStaticArray& m_Ref;
			size_t m_Pos;
		};

#pragma endregion 

		// TODO: Finish the big 5
		CustomStaticArray()
			: m_pHead()
			, m_CurrentSize()
			, m_MaxSize()
			, m_pVirtualAddressSpaces()
		{
		}

		template<size_t Count>
		CustomStaticArray(const std::array<T, Count>& arrResource)
			: m_pHead(const_cast<T*>(&arrResource[0])) // bruh kinda bad
			, m_CurrentSize(arrResource.size())
			, m_MaxSize(arrResource.max_size())
			, m_pVirtualAddressSpaces()
		{
		}

		CustomStaticArray(pointer pHeadOfMem, size_t maxSize, size_t currentSize = 0)
			: m_pHead(pHeadOfMem)
			, m_CurrentSize(currentSize)
			, m_MaxSize(maxSize)
			, m_pVirtualAddressSpaces()
		{
		}

		/// <summary>
		/// Construct a hard ref for any memory that wishes to hold its life time to this handle
		/// </summary>
		/// <param name="pSmartMem"></param>
		/// <param name="currentSize"></param>
		/// <param name="maxSize"></param>
		CustomStaticArray(const SharedPtr<T> pSmartMem, size_t maxSize, size_t currentSize = 0)
			: m_pHead(pSmartMem.get())
			, m_pStrongRef(pSmartMem)
			, m_CurrentSize(currentSize)
			, m_MaxSize(maxSize)
			, m_pVirtualAddressSpaces()
		{
		}

		CustomStaticArray(const CustomStaticArray& other)
			: m_pHead(other.m_pHead)
			, m_pStrongRef(other.m_pStrongRef)
			, m_CurrentSize(other.m_CurrentSize)
			, m_MaxSize(other.m_MaxSize)
			, m_pVirtualAddressSpaces(other.m_pVirtualAddressSpaces)
		{
		}

		CustomStaticArray(CustomStaticArray&& other) noexcept
			: m_pHead(std::move(other.m_pHead))
			, m_pStrongRef(std::move(other.m_pStrongRef))
			, m_CurrentSize(std::move(other.m_CurrentSize))
			, m_MaxSize(std::move(other.m_MaxSize))
			, m_pVirtualAddressSpaces(std::move(other.m_pVirtualAddressSpaces))
		{
		}

		CustomStaticArray& operator=(const CustomStaticArray& other)
		{
			if (this != &other)
			{
				m_pHead = other.m_pHead;
				m_pStrongRef = other.m_pStrongRef;
				m_CurrentSize = other.m_CurrentSize;
				m_MaxSize = other.m_MaxSize;
				m_pVirtualAddressSpaces = other.m_pVirtualAddressSpaces;
			}

			return *this;
		}

		CustomStaticArray& operator=(CustomStaticArray&& other) noexcept
		{
			if (this != &other)
			{
				m_pHead = std::move(other.m_pHead);
				m_pStrongRef = std::move(other.m_pStrongRef);
				m_CurrentSize = std::move(other.m_CurrentSize);
				m_MaxSize = std::move(other.m_MaxSize);
				m_pVirtualAddressSpaces = std::move(other.m_pVirtualAddressSpaces);
			}

			return *this;
		}

		auto& operator[](size_t pos)
		{
			return *(m_pHead + pos);
		}

		[[nodiscard]] size_t size() const { return m_CurrentSize; }
		[[nodiscard]] size_t max_size() const { return m_MaxSize; }
		[[maybe_unused]] constexpr auto erase(size_t pos) noexcept->Iterator;
		[[maybe_unused]] constexpr auto erase(const Iterator& pos) noexcept->Iterator;
		[[maybe_unused]] constexpr auto erase(Iterator&& pos) noexcept->Iterator;
		[[nodiscard]] constexpr auto data() const noexcept ->pointer { return m_pHead; }
		[[nodiscard]] constexpr auto at(size_t pos) noexcept(false)->reference;
		[[maybe_unused]] constexpr auto insert(const value_type& value) noexcept -> std::optional<Iterator>;
		[[maybe_unused]] constexpr auto insert(value_type&& value) noexcept -> std::optional<Iterator>;
		[[nodiscard]] constexpr auto begin() const { return Iterator(m_pHead); }
		[[nodiscard]] constexpr auto end() const { return Iterator(&m_pHead[m_MaxSize]); }
		//constexpr auto cbegin() const { return Const_Iterator(*this, 0); }
		//constexpr auto cend() const { return Const_Iterator(*this, Count); }

	private:

		/// <summary>
		/// Pointer point to the first element in array
		/// assume that the array doesn't get destroyed
		/// </summary>
		pointer m_pHead;


		SharedPtr<T> m_pStrongRef;

		size_t m_CurrentSize;
		size_t m_MaxSize;

		// address spaces
		std::queue<pointer> m_pVirtualAddressSpaces;
	};

	template <typename T>
	constexpr auto CustomStaticArray<T>::erase(size_t pos) noexcept -> Iterator
	{
		if (pos < m_CurrentSize)
			return erase(Iterator(m_pHead + pos));
		return end();
	}

	template <typename T>
	constexpr auto CustomStaticArray<T>::erase(const Iterator& pos) noexcept -> Iterator
	{
		const Iterator endIt{ end() };
		if (pos != endIt)
		{
			pointer pointedMem{ pos.m_Ptr };
			m_pVirtualAddressSpaces.push(pointedMem);
			pointedMem->~value_type();
			--m_CurrentSize;
			return Iterator(pointedMem);
		}
		return endIt;
	}

	template <typename T>
	constexpr auto CustomStaticArray<T>::erase(Iterator&& pos) noexcept -> Iterator
	{
		const Iterator endIt{ end() };
		if (pos != endIt)
		{
			pointer pointedMem{ pos.m_Ptr };
			m_pVirtualAddressSpaces.push(pointedMem);
			pointedMem->~value_type();
			--m_CurrentSize;
			return Iterator(pointedMem);
		}
		return endIt;
	}

	template <typename T>
	constexpr auto CustomStaticArray<T>::at(size_t pos) noexcept(false) -> value_type&
	{
		if (pos < m_CurrentSize)
			return this[pos];
		throw std::out_of_range("array accessing is out of range");
	}

	template <typename T>
	constexpr auto CustomStaticArray<T>::insert(const value_type& value) noexcept -> std::optional<Iterator>
	{
		if (m_CurrentSize < m_MaxSize)
		{
			pointer emptyAddress{};
			if (!m_pVirtualAddressSpaces.empty())
			{
				emptyAddress = m_pVirtualAddressSpaces.front();
				m_pVirtualAddressSpaces.pop();
			}
			else
				emptyAddress = m_pHead + m_CurrentSize;

			new (emptyAddress) value_type(value);
			++m_CurrentSize;
			return Iterator(emptyAddress);
		}

		return {};
	}

	template<typename T>
	constexpr auto CustomStaticArray<T>::insert(value_type&& value) noexcept -> std::optional<Iterator>
	{
		if (m_CurrentSize < m_MaxSize)
		{
			pointer emptyAddress{};
			if (!m_pVirtualAddressSpaces.empty())
			{
				emptyAddress = m_pVirtualAddressSpaces.front();
				m_pVirtualAddressSpaces.pop();
			}
			else
				emptyAddress = m_pHead + m_CurrentSize;

			*emptyAddress = std::move(value);
			++m_CurrentSize;
			return Iterator(emptyAddress);
		}

		return {};
	}
}

#endif
