// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#if defined(_WIN32)

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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#endif

#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <any>
#include <chrono>
#include <cassert>

#include "../Helpers/CustomTypes.h"
#include "../EngineContext.h"
#include "../Helpers/Delegate.h"
#include "../Helpers/Event.h"

#endif //PCH_H
