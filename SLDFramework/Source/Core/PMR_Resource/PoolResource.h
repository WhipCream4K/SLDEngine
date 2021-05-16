
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

	private:

		void DoShifBuffer(size_t newSize);
		void* Allocate(void* dst, size_t bytes, size_t alignment);

	private:

		std::vector<uint8_t> m_Buffer;
		std::queue<uint8_t*> m_EmptyAddressSpace;
		size_t m_CurrentSize;
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
		//m_Buffer.resize(Size, 0);
		m_pHead = m_Buffer.data();
	}

	template <size_t Size>
	void* PoolResource<Size>::do_allocate(size_t _Bytes, size_t _Align)
	{
		if (_Bytes <= size_t(std::abs((long long)(m_Buffer.capacity() - m_Buffer.size()))))
		{
			if (m_EmptyAddressSpace.empty())
				return Allocate(m_pHead, _Bytes, _Align);

			void* ptr{ m_EmptyAddressSpace.front() };
			m_EmptyAddressSpace.pop();

			return Allocate(ptr, _Bytes, _Align);
		}

		DoShifBuffer(_Bytes);
		
		return Allocate(m_pHead, _Bytes, _Align);
	}

	template <size_t Size>
	void PoolResource<Size>::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
	{
		// TODO: Maybe fragment
		_Align;
		m_EmptyAddressSpace.emplace(static_cast<uint8_t*>(_Ptr));
		std::memset(_Ptr, 0, _Bytes);
	}

	template <size_t Size>
	bool PoolResource<Size>::do_is_equal(const memory_resource& _That) const noexcept
	{
		return this == &_That;
	}

	template <size_t Size>
	void PoolResource<Size>::DoShifBuffer(size_t newSize)
	{
		const size_t allocSize{ (newSize + 7) & (-8) };
		m_Buffer.reserve((m_Buffer.size() + allocSize) * 8); // always increase in multiple of 8
		m_pHead = m_Buffer.data() + m_Buffer.size();
	}

	template <size_t Size>
	void* PoolResource<Size>::Allocate(void* dst, size_t bytes, size_t alignment)
	{
		//m_pHead += bytes;
		//m_CurrentSize += bytes;
		alignment;

		if(m_pHead == dst)
		{
			m_pHead += bytes;
			m_Buffer.resize(m_Buffer.size() + bytes);
		}

		//if (std::align(alignment, bytes, outPtr, bufferAlignSize))
		//{
		//	if (dst == m_pHead)
		//		m_pHead += bytes;
		//	m_CurrentSize += bytes;
		//	return outPtr;
		//}

		return dst;
	}
}


#endif
