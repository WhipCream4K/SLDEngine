
#ifndef SLDFRAMEWORK_SPATIALRESOURCE_H
#define SLDFRAMEWORK_SPATIALRESOURCE_H

#include "../Base.h"
#include <deque>

namespace SLD
{
	template<size_t Val>
	constexpr bool IsMultipleOf2{ Val && (!(Val & (Val - 1))) };

	struct Type
	{
		using intergral = size_t;
	};

	template<size_t BufferSize>
	class UnsafeSpatialResource : public std::pmr::memory_resource
	{
	public:

		// should be align-able as 8 bytes
		// always allocate more space then needed because it will allocate
		// in parabolic manner
		template<typename = std::enable_if_t <IsMultipleOf2<BufferSize>>>
		UnsafeSpatialResource();

		void* do_allocate(size_t _Bytes, size_t _Align) override;
		void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override;
		bool do_is_equal(const memory_resource& _That) const noexcept override;

	private:

		void DoShifBuffer();
		void* Allocate(size_t bytes, size_t alignment);

	protected:

		std::vector<uint8_t> m_Buffer;
		uint8_t* m_pHead{};
	};

	template <size_t BufferSize>
	template <typename>
	UnsafeSpatialResource<BufferSize>::UnsafeSpatialResource()
		: m_Buffer()
		, m_pHead()
	{
		m_Buffer.reserve(BufferSize);
		m_pHead = m_Buffer.data();
	}

	template <size_t BufferSize>
	void* UnsafeSpatialResource<BufferSize>::do_allocate(size_t _Bytes, size_t _Align)
	{
		if (_Bytes < m_Buffer.size())
		{
			return Allocate(_Bytes, _Align);
		}

		DoShifBuffer();

		return Allocate(_Bytes, _Align);
	}

	template <size_t BufferSize>
	void UnsafeSpatialResource<BufferSize>::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
	{
		// Since we can't control the pointers retrieved by do_allocate so we going to leave this empty
		// NOTE: the buffer can still destroy itself when out of scope though so it's fine
		// TODO: maybe deal with this later
		_Ptr;
		_Align;
		_Bytes;
	}

	template <size_t BufferSize>
	bool UnsafeSpatialResource<BufferSize>::do_is_equal(const memory_resource& _That) const noexcept
	{
		return this == &_That;
	}

	template <size_t BufferSize>
	void UnsafeSpatialResource<BufferSize>::DoShifBuffer()
	{
		const size_t newSize{ m_Buffer.size() * 2 };
		m_Buffer.reserve(newSize); // always increase in power of 2
		m_pHead = m_Buffer.data() + newSize - 1;
	}

	template <size_t BufferSize>
	void* UnsafeSpatialResource<BufferSize>::Allocate(size_t bytes, size_t alignment)
	{
		void* outPtr{ m_pHead };
		size_t bufferAlignSize{};
		if (std::align(alignment, bytes, outPtr, bufferAlignSize))
		{
			m_pHead += bytes;
			return outPtr;
		}

		return nullptr;
	}
}

#endif
