#include "DX12Resource.h"

#ifdef HAS_DX12

void DX12Resource::Init(bool shouldUseWARP)
{
	const auto dxgiAdapter{ GetAdapter(shouldUseWARP) };
	m_MainDevice = CreateDevice(dxgiAdapter);

	m_DirectCommandQueue = std::make_shared<DX12CommandQueue>(m_MainDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
	m_CopyCommandQueue = std::make_shared<DX12CommandQueue>(m_MainDevice, D3D12_COMMAND_LIST_TYPE_COPY);
	m_ComputeCommandQueue = std::make_shared<DX12CommandQueue>(m_MainDevice, D3D12_COMMAND_LIST_TYPE_COMPUTE);

	EnableDebugLayer();
}

RefPtr<DX12CommandQueue> DX12Resource::GetCommandQueue(D3D12_COMMAND_LIST_TYPE type) const
{
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		return m_ComputeCommandQueue;
	case D3D12_COMMAND_LIST_TYPE_COPY:
		return m_CopyCommandQueue;
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		return m_DirectCommandQueue;
	default: break;
	}

	return nullptr;
}

void DX12Resource::FlushCommands()
{
	m_DirectCommandQueue->Flush();
	m_CopyCommandQueue->Flush();
	m_ComputeCommandQueue->Flush();
}

ComPtr<IDXGIAdapter4> DX12Resource::GetAdapter(bool useWarp)
{
	ComPtr<IDXGIFactory4> dxgiFactory{};
	UINT createFactoryFlags{};

#ifdef _DEBUG
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	ComPtr<IDXGIAdapter1> dxgiAdapter1{};
	ComPtr<IDXGIAdapter4> dxgiAdapter4{};

	if (useWarp)
	{
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));

		ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
	}
	else
	{
		size_t maxDedicatedVideoMemory{};
		for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1{};
			dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the provided adapter could create d3d12Device without creating it
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0
				&& SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
					D3D_FEATURE_LEVEL_11_0,
					__uuidof(ID3D12Device), nullptr))
				&& dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
			{
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
	}

	return dxgiAdapter4;
}

ComPtr<ID3D12Device2> DX12Resource::CreateDevice(const ComPtr<IDXGIAdapter4>& dxgiAdapter)
{
	ComPtr<ID3D12Device2> device{};

	ThrowIfFailed(D3D12CreateDevice(
		dxgiAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&device)));

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> pInfoQueue{};
	if (SUCCEEDED(device.As(&pInfoQueue)))
	{
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

		D3D12_MESSAGE_SEVERITY severities[]
		{
			D3D12_MESSAGE_SEVERITY_INFO
		};

		// Suppress individual message by their ID
		D3D12_MESSAGE_ID denyIds[]
		{
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
			D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
		};

		D3D12_INFO_QUEUE_FILTER newFilter{};

		newFilter.DenyList.NumSeverities = _countof(severities);
		newFilter.DenyList.pSeverityList = severities;
		newFilter.DenyList.NumIDs = _countof(denyIds);
		newFilter.DenyList.pIDList = denyIds;

		ThrowIfFailed(pInfoQueue->PushStorageFilter(&newFilter));
	}
#endif

	return device;
}

void DX12Resource::EnableDebugLayer()
{
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugInterface{};

	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));

	debugInterface->EnableDebugLayer();
#endif
}

#endif
