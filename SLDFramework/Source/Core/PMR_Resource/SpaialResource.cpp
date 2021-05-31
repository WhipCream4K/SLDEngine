#include "SpaialResource.h"

SLD::SpaialResource::SpaialResource(size_t bufferSize)
	: m_Buffer()
	, m_Observer()
	, m_pHead()
	, m_pCurrent()
{
	m_Buffer.reserve(bufferSize);
	m_pHead = m_Buffer.data();
	m_pCurrent = m_pHead;
}

void SLD::SpaialResource::DoDeAllocate(void* ptr, size_t size)
{

}

uint8_t* const& SLD::SpaialResource::GetHead() const noexcept
{
	return m_pHead;
}

void SLD::SpaialResource::IncreaseBuffer(size_t newSize)
{
	const size_t oldSize{ m_Buffer.size() };
	const size_t allocSize{ (newSize + 7) & (-8) };
	m_Buffer.reserve(allocSize * 8); // always increase in multiple of 8
	m_pHead = m_Buffer.data();
	m_pCurrent = m_pHead + oldSize;
}
