
#ifndef SLDFRAMEWORK_POOLRESOURCE_H
#define SLDFRAMEWORK_POOLRESOURCE_H

#include "UnsafeSpatialResource.h"
#include "../Base.h"

namespace SLD
{
	template<size_t Size>
	class PoolResource : public std::pmr::memory_resource
	{
	public:

		template<typename = std::enable_if_t <IsMultipleOf2<Size>>>
		PoolResource();

		void* do_allocate(size_t _Bytes, size_t _Align = sizeof(std::max_align_t)) override;
		void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override;
		bool do_is_equal(const memory_resource& _That) const noexcept override;

		[[nodiscard]] void*& AllocFromTable(size_t bytes, size_t align = sizeof(max_align_t));
		void DeAllocFromTable(void* ptr, size_t bytes, size_t align);

		uint8_t* const& GetBufferHead() const noexcept;

	private:

		void DoShifBuffer(size_t newSize);
		void* Allocate(void* dst, size_t bytes, size_t alignment, size_t& offSet);
		void ResetPointerTable();

		size_t GetByteOffsetFromTable(void* ptr);
		void* NewAllocate(void* dst, size_t bytes, size_t alignment);

	private:

		struct PointerOffset
		{
			void* ptr{}; // ref from buffer
			size_t bytesOffsetFromHead{};
		};

		std::vector<uint8_t> m_Buffer;
		std::queue<uint8_t*> m_EmptyAddressSpace;
		std::vector<PointerOffset> m_RefTable;
		size_t m_CurrentSize;
		uint8_t* m_pCurrentPtr{};
		uint8_t* m_pHead{};
	};

	template <size_t Size>
	template <typename>
	PoolResource<Size>::PoolResource()
		: m_Buffer()
		, m_EmptyAddressSpace()
		, m_CurrentSize()
	{
		m_Buffer.reserve(Size);
		m_pCurrentPtr = m_Buffer.data();
		m_pHead = m_Buffer.data();
	}

	template <size_t Size>
	void* PoolResource<Size>::do_allocate(size_t _Bytes, size_t _Align)
	{
		// TODO: FIX ME
		_Bytes;
		_Align;

		if (_Bytes <= size_t(std::abs((long long)(m_Buffer.capacity() - m_Buffer.size()))))
		{
			if (m_EmptyAddressSpace.empty())
				return NewAllocate(m_pCurrentPtr, _Bytes, _Align);

			void* ptr{ m_EmptyAddressSpace.front() };
			m_EmptyAddressSpace.pop();

			return NewAllocate(ptr, _Bytes, _Align);
		}

		DoShifBuffer(_Bytes);

		return NewAllocate(m_pCurrentPtr, _Bytes, _Align);
	}

	template <size_t Size>
	void PoolResource<Size>::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
	{
		// TODO: Maybe fragment
		_Align;
		m_EmptyAddressSpace.emplace(static_cast<uint8_t*>(_Ptr));
		_Bytes;
		std::memset(_Ptr, 0, _Bytes);
	}

	template <size_t Size>
	bool PoolResource<Size>::do_is_equal(const memory_resource& _That) const noexcept
	{
		return this == &_That;
	}

	template <size_t Size>
	void*& PoolResource<Size>::AllocFromTable(size_t bytes, size_t align)
	{
		if (bytes <= size_t(std::abs((long long)(m_Buffer.capacity() - m_Buffer.size()))))
		{
			if (m_EmptyAddressSpace.empty())
			{
				size_t offSet{};
				void* out{ Allocate(m_pCurrentPtr,bytes,align,offSet) };
				return m_RefTable.emplace_back(PointerOffset{ out,offSet }).ptr;
			}

			void* ptr{ m_EmptyAddressSpace.front() };
			m_EmptyAddressSpace.pop();

			return m_RefTable.emplace_back(PointerOffset{ ptr,GetByteOffsetFromTable(ptr) }).ptr;
		}

		DoShifBuffer(bytes);

		// I know that it has more space
		size_t offSet{};
		void* out{ Allocate(m_pCurrentPtr,bytes,align,offSet) };
		return m_RefTable.emplace_back(PointerOffset{ out,offSet }).ptr;
	}

	template <size_t Size>
	void PoolResource<Size>::DeAllocFromTable(void* ptr, size_t bytes, size_t align)
	{
		// TODO: maybe don't deallocate from this table but leave empty
		m_RefTable.erase(std::remove_if(m_RefTable.begin(), m_RefTable.end(), [&ptr](const PointerOffset& item)
			{
				return ptr == item.ptr;
			}));

		do_deallocate(ptr, bytes, align);
	}

	template <size_t Size>
	uint8_t* const& PoolResource<Size>::GetBufferHead() const noexcept
	{
		return m_pHead;
	}

	template <size_t Size>
	void PoolResource<Size>::DoShifBuffer(size_t newSize)
	{
		const size_t allocSize{ (newSize + 7) & (-8) };
		m_Buffer.reserve((m_Buffer.size() + allocSize) * 8); // always increase in multiple of 8
		m_pHead = m_Buffer.data();
		m_pCurrentPtr = m_pHead + m_Buffer.size();
		//ResetPointerTable();
	}

	template <size_t Size>
	void* PoolResource<Size>::Allocate(void* dst, size_t bytes, size_t alignment, size_t& offSet)
	{
		//m_pCurrentPtr += bytes;
		//m_CurrentSize += bytes;
		alignment;

		if (m_pCurrentPtr == dst)
		{
			// NOTE: now I feel bad by doing this
			uint8_t* oldHead{ m_Buffer.data() };
			size_t sizeBeforeUpdate{ m_Buffer.size() };
			offSet = sizeBeforeUpdate;
			m_Buffer.resize(m_Buffer.size() + bytes);
			void* out{ m_Buffer.data() + sizeBeforeUpdate };
			if (oldHead != m_Buffer.data())
				ResetPointerTable();
			m_pCurrentPtr += bytes;
			return out;
		}

		//if (std::align(alignment, bytes, outPtr, bufferAlignSize))
		//{
		//	if (dst == m_pCurrentPtr)
		//		m_pCurrentPtr += bytes;
		//	m_CurrentSize += bytes;
		//	return outPtr;
		//}

		return dst;
	}

	template <size_t Size>
	void PoolResource<Size>::ResetPointerTable()
	{
		for (auto& ptrOffset : m_RefTable)
		{
			ptrOffset.ptr = m_Buffer.data() + ptrOffset.bytesOffsetFromHead;
		}
	}

	template <size_t Size>
	size_t PoolResource<Size>::GetByteOffsetFromTable(void* ptr)
	{
		auto fIt = std::find_if(m_RefTable.begin(), m_RefTable.end(), [&ptr](const PointerOffset& item)
			{
				return item.ptr == ptr;
			});

		if (fIt != m_RefTable.end())
			return fIt->bytesOffsetFromHead;

		return {};
	}

	template <size_t Size>
	void* PoolResource<Size>::NewAllocate(void* dst, size_t bytes, size_t alignment)
	{
		alignment;

		if (m_pCurrentPtr == dst)
		{
			size_t oldSize{ m_Buffer.size() };
			m_Buffer.resize(oldSize + bytes);
			void* out{ m_Buffer.data() + oldSize };
			m_pHead = m_Buffer.data();
			m_pCurrentPtr = m_pHead + m_Buffer.size() + bytes;
			return out;
		}

		return dst;
	}
}


#endif
