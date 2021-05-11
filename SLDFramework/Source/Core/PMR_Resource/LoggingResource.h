
#ifndef SLDFRAMEWORK_LOGGINGRESOURCE_H
#define SLDFRAMEWORK_LOGGINGRESOURCE_H

#include "../Base.h"

namespace SLD
{
	class LoggingResource : public std::pmr::memory_resource
	{
	public:
		
		explicit LoggingResource(std::pmr::memory_resource* upStream);

		void* do_allocate(size_t _Bytes, size_t _Align) override;
		void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override;
		bool do_is_equal(const memory_resource& _That) const noexcept override;

	private:
		
		std::pmr::memory_resource* m_UpStream;
	};
}

#endif
