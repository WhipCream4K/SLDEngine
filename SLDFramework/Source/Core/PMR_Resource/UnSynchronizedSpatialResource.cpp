#include "UnSynchronizedSpatialResource.h"

SLD::UnSynchronizedSpatialResource::UnSynchronizedSpatialResource(size_t bufferSize)
	: m_pHead()
	//, m_Observer()
	, m_pCurrent()
	, m_Buffer()
{
	m_Buffer.reserve(bufferSize);
	m_pHead = m_Buffer.data();
	m_pCurrent = m_pHead;
}

void* SLD::UnSynchronizedSpatialResource::do_allocate(size_t _Bytes, size_t _Align)
{
	const size_t sizeFromHead{ m_Buffer.size() };

	if(sizeFromHead + _Bytes <= m_Buffer.capacity())
		return Alloc(_Bytes, _Align);
	
	IncreaseBuffer(sizeFromHead + _Bytes);

	return Alloc(_Bytes, _Align);
}

void SLD::UnSynchronizedSpatialResource::do_deallocate(void* _Ptr, size_t _Bytes, [[maybe_unused]] size_t _Align)
{
	uint8_t* castPtr{ (uint8_t*)_Ptr };
	uint8_t* nextHeadPointer{ castPtr + _Bytes };
	const size_t moveSize{ size_t(abs(nextHeadPointer - m_pCurrent)) };

	// Move all the bytes in the buffer up
	std::memcpy(nextHeadPointer, castPtr, moveSize);

	bool isFound{};
	size_t diffSizeToNextPointer{};
	for (auto it = m_Observer.begin(); it != m_Observer.end(); ++it)
	{
		// The list after the pointer will be adjusted
		if(isFound)
		{
			(*it) -= diffSizeToNextPointer;
			continue;
		}
		
		if (*it == _Ptr)
		{
			const auto nextIt{ std::next(it, 1) };

			if(nextIt != m_Observer.end())
			{
				diffSizeToNextPointer = size_t(abs(castPtr - (*nextIt)));
				isFound = true;
			}
			
			it = m_Observer.erase(it);	
		}
	}

	m_Buffer.resize(m_Buffer.size() - _Bytes);
	m_pHead = m_Buffer.data();
	m_pCurrent -= diffSizeToNextPointer;
}

bool SLD::UnSynchronizedSpatialResource::do_is_equal(const memory_resource& _That) const noexcept
{
	return this == &_That;
}

uint8_t* const& SLD::UnSynchronizedSpatialResource::GetHead() const noexcept
{
	return m_pHead;
}

size_t SLD::UnSynchronizedSpatialResource::GetUsedSize() const noexcept
{
	return m_Buffer.size();
}

void* SLD::UnSynchronizedSpatialResource::Alloc(size_t bytes, [[maybe_unused]] size_t alignment)
{
	auto& outPtr{ m_Observer.emplace_back(m_pCurrent) };
	m_pCurrent += bytes;
	return outPtr;
}

void SLD::UnSynchronizedSpatialResource::IncreaseBuffer(size_t newSize)
{
	const size_t oldSize{ m_Buffer.size() };
	const size_t allocSize{ (newSize + 7) & (-8) };
	m_Buffer.reserve(allocSize * 8); // always increase in multiple of 8
	m_pHead = m_Buffer.data();
	m_pCurrent = m_pHead + oldSize;
}
