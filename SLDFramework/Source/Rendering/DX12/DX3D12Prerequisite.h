
#ifndef SLDFARMEWORK_DX3D12_PREREQUISITE_H
#define SLDFARMEWORK_DX3D12_PREREQUISITE_H

// DirectX 12 headers
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

// DirectX 12 Helpers
#include "d3dx12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#ifdef _WIN32
//#define HAS_DX12
#endif


#endif
