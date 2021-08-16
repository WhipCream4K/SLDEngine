

#ifndef SLDFRAMEWORK_MEMALLOCATOR_H
#define SLDFRAMEWORK_MEMALLOCATOR_H

#include "Base.h"

namespace SLD
{
	class MemAllocator
	{
	public:

		template<typename T>
		static SharedPtr<std::remove_all_extents_t<T>> NewAligned()
		{
			using value_type = std::remove_all_extents_t<T>;
			
		}
	};
}

#endif