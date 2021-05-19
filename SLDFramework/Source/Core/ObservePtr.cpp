#include "ObservePtr.h"

//SLD::ObservePtr::ObservePtr(std::add_pointer_t<void> const& voidException, size_t offset)
//	: m_BufferStart((uint8_t*&)(voidException))
//	, m_Offset(offset)
//{
//}
//
//SLD::ObservePtr::ObservePtr(std::add_pointer_t<uint8_t> const& ptrValue, size_t offset)
//	: m_BufferStart(ptrValue)
//	, m_Offset(offset)
//{
//}
//
//SLD::ObservePtr::ObservePtr(const ObservePtr& other)
//	: m_BufferStart(other.m_BufferStart)
//	, m_Offset(other.m_Offset)
//{
//}
//
//SLD::ObservePtr& SLD::ObservePtr::operator=(const ObservePtr& other)
//{
//	if (this != &other)
//	{
//		const_cast<uint8_t*&>(m_BufferStart) = other.m_BufferStart;
//		m_Offset = other.m_Offset;
//	}
//}
//
//SLD::ObservePtr::ObservePtr(ObservePtr&& other) noexcept
//	: m_BufferStart(std::move(other.m_BufferStart))
//	, m_Offset(std::move(other.m_Offset))
//{
//}
//
//SLD::ObservePtr& SLD::ObservePtr::operator=(ObservePtr&& other) noexcept
//{
//	if(this !=&other)
//	{
//		const_cast<uint8_t*&>(m_BufferStart) = std::move(other.m_BufferStart);
//		m_Offset = std::move(other.m_Offset);
//	}
//	return *this;
//	
//}
//
//SLD::ObservePtr::~ObservePtr()
//{
//}

