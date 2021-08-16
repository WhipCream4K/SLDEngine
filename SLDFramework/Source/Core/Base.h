
#ifndef SLDFRAMEWORK_BASE_H
#define SLDFRAMEWORK_BASE_H

#include <functional>
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <stdexcept>
#include <any>
#include <chrono>
#include <cassert>
#include <memory_resource>

#include "CustomTypes.h"
#include "EngineContext.h"
#include "../Debugging/TSLogger.h"
#include "../Math/RealTimeMath.h"
#include "ObservePtr.h"
//#include "../Helpers/utils.h"
//#include "../Miscellaneous/RealTimeMath.h"

//void* operator new(size_t size);
//void operator delete(void* p, size_t size);

namespace SLD
{
	
	template<typename T>
	T* New(size_t size = sizeof(T))
	{
		void* p = malloc(size);
		if (!p)
			throw std::bad_alloc();
		
		SLD_LOG_TRACE_HEAP_ALLOC(size);

		return static_cast<T*>(p);
	}

	template<typename T>
	SharedPtr<T> MakeShared()
	{
		return SharedPtr<T>{New<T>(),[](T* pointer)
		{
				delete pointer;
				pointer = nullptr;
		}};
	}

	template<typename T,typename Dx>
	SharedPtr<T> MakeShared(Dx deleteFunc)
	{
		return SharedPtr<T>{New<T>(), deleteFunc};
	}
}

#define SLD_SUPPORT_SFML	1

#endif