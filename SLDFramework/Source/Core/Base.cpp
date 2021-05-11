#include "Base.h"

//void* operator new(size_t size)
//{
//	void* p{ malloc(size) };
//	if (!p)
//		throw std::bad_alloc();
//
//	//SLD_LOG_TRACE_HEAP_ALLOC(size);
//
//	return p;
//}
//
//void operator delete(void* p, size_t size)
//{
//	size;
//	//SLD_LOG_UNTRACE_HEAP_ALLOC(size);
//	free(p);
//}