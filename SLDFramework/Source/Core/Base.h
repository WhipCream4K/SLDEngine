
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
#include "../Helpers/utils.h"
#include "../Debugging/TSLogger.h"
#include "../Miscellaneous/RealTimeMath.h"

template<typename SingletonType>
SingletonType Instance()
{
	return SingletonType::Instance();
}

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
	RefPtr<T> MakeShared()
	{
		return RefPtr<T>{New<T>(),[](T* pointer)
		{
				delete pointer;
				pointer = nullptr;
		}};
	}

	template<typename T,typename Dx>
	RefPtr<T> MakeShared(Dx deleteFunc)
	{
		return RefPtr<T>{New<T>(), deleteFunc};
	}
}



#endif