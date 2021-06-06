#ifndef SLDFRAMEWORK_UTILS_H
#define SLDFRAMEWORK_UTILS_H

#include <type_traits>

//void* operator new(size_t size);
//void operator delete(void* p,size_t size);

namespace SLD
{
	template<typename T,
	typename = std::enable_if_t<std::is_integral_v<T>>>
	constexpr const T& clamp(const T& val,const T& lo, const T& hi)
	{
		if (val > hi)
			return hi;
		if (val < lo)
			return  lo;

		return val;
	}
}

#endif

