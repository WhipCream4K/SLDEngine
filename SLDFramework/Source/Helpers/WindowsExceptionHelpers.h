
#ifndef SLDFRAMEWORK_WINDOWS_EXCEPTION_HELPERS_H
#define SLDFRAMEWORK_WINDOWS_EXCEPTION_HELPERS_H

#include <string>
#include <typeinfo>

static void ThrowIfFailed(HRESULT rs)
{
	if (FAILED(rs))
	{
		throw std::exception();
	}
}

#endif
