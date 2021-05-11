
#ifndef SLDFRAMEWORK_WINPREREQUISTITE_H
#define SLDFRAMEWORK_WINPREREQUISTITE_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <wrl.h>
using namespace Microsoft::WRL;

#include "../Helpers/WindowsExceptionHelpers.h"

#endif


#endif
