#include "RenderBuffer.h"

SLD::RenderBuffer::RenderBuffer()
	: m_MainBuffer(std::make_unique<uint8_t[]>(2))
	, m_CurrentBufferSize()
	, m_BufferCapacity(2)
{
}

SLD::RenderBuffer::RenderBuffer(size_t startSize)
	: m_MainBuffer(std::make_unique<uint8_t[]>(startSize))
	, m_CurrentBufferSize()
	, m_BufferCapacity(startSize)
{
}

void SLD::RenderBuffer::PushRenderElement(uint8_t* buffer, size_t size)
{
	if (size + m_CurrentBufferSize > m_BufferCapacity)
	{
		Resize(size + m_CurrentBufferSize);
	}

	std::copy(buffer, buffer + size, m_MainBuffer.get() + m_CurrentBufferSize);
	m_CurrentBufferSize += size;
	m_RenderElemCnt++;
}

void SLD::RenderBuffer::Resize(size_t count)
{
	//m_CurrentBufferSize = count;

	if (count > m_BufferCapacity)
	{
		const size_t multipleOf8{ (count + 7) & (-8) };
		ShifBuffer(multipleOf8 * 8);
	}
}

uint8_t* SLD::RenderBuffer::GetBufferStart() const noexcept
{
	return m_MainBuffer.get();
}

size_t SLD::RenderBuffer::GetBufferCapacity() const noexcept
{
	return m_BufferCapacity;
}

size_t SLD::RenderBuffer::GetBufferSize() const noexcept
{
	return m_CurrentBufferSize;
}

void SLD::RenderBuffer::Clear()
{
	m_CurrentBufferSize = 0;
	m_RenderElemCnt = 0;
}

uint32_t SLD::RenderBuffer::GetElementCnt() const noexcept
{
	return m_RenderElemCnt;
}

void SLD::RenderBuffer::EraseWatcherThenShift(uint8_t* where, size_t dataSize)
{
	m_RenderElemCnt--;

	size_t totalMoveSize{ dataSize + sizeof(RenderIdentifier) + sizeof(void*) };

	bool isFound{};
	bool isLast{};
	for (auto it = m_REWatcher.begin(); it != m_REWatcher.end(); ++it)
	{
		if (isFound)
		{
			auto& temp = *it;
			temp -= dataSize;
			continue;
		}

		if (where == (*it))
		{
			isFound = true;
			it = m_REWatcher.erase(it);
			if (it == m_REWatcher.end())
			{
				isLast = true;
				break;
			}
		}
	}

	if (!isLast)
	{
		uint8_t* bottom{ where + dataSize };
		std::copy_n(bottom,
			size_t(abs(bottom - (m_MainBuffer.get() + m_CurrentBufferSize))),
			where - sizeof(RenderIdentifier) - sizeof(void*));
	}


	m_CurrentBufferSize -= totalMoveSize;
}

void SLD::RenderBuffer::ShifBuffer(size_t newSize)
{
	uint8_t* newLocal{ new uint8_t[newSize]{} };
	std::copy_n(m_MainBuffer.get(), m_BufferCapacity, newLocal);

	for (auto& ptr : m_REWatcher)
	{
		const size_t diffFromHead = size_t(abs(m_MainBuffer.get() - ptr));
		ptr = newLocal + diffFromHead;
	}

	m_MainBuffer.reset(newLocal);
	m_BufferCapacity = newSize;
}
