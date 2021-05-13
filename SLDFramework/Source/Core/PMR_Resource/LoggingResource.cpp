#include "LoggingResource.h"

SLD::LoggingResource::LoggingResource(std::pmr::memory_resource* upStream)
	: m_UpStream(upStream)
{
	assert(upStream != nullptr);
}

void* SLD::LoggingResource::do_allocate(size_t _Bytes, size_t _Align)
{
	SLD_LOG_TRACE_USED_HEAP(_Bytes);
	return m_UpStream->allocate(_Bytes, _Align);
}

void SLD::LoggingResource::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
{
	SLD_LOG_UNTRACE_USED_HEAP(_Bytes);
	m_UpStream->deallocate(_Ptr, _Bytes, _Align);
}

bool SLD::LoggingResource::do_is_equal(const memory_resource& _That) const noexcept
{
	return this != &_That;
}
