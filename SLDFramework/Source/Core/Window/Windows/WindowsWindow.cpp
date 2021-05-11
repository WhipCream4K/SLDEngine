#include "WindowsWindow.h"
#include "../../Miscellaneous/DX12Resource.h"

#if defined(_WIN32) && defined(HAS_DX12)

WindowsWindow::WindowsWindow(const ComPtr<ID3D12CommandQueue>& commandQueue, HWND windowHandle, uint32_t width,
	uint32_t height)
{

	m_WindowSwapChain = CreateSwapChain(commandQueue, windowHandle, width, height);
	m_IsTearingSupported = CheckTearingSupport();

	auto device{ DX12Resource::Instance().GetDevice() };

	m_RTVDescriptorHeap = CreateDescriptorHeap(device,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		1);

	m_RTVSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	m_DSVDescriptorHeap = CreateDescriptorHeap(device,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1);

	m_DSVSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	m_CurrentBackBufferIdx = m_WindowSwapChain->GetCurrentBackBufferIndex();

	UpdateRenderTargetViews(device);
	UpdateDepthStencilView(device, width, height);
}

const ComPtr<ID3D12Resource> WindowsWindow::GetBackBufferResource(UINT backBufferIdx) const
{
	return m_BackBufferResource[backBufferIdx];
}

const ComPtr<IDXGISwapChain4> WindowsWindow::GetSwapChain() const
{
	return m_WindowSwapChain;
}

D3D12_CPU_DESCRIPTOR_HANDLE WindowsWindow::GetRenderTargetView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE{
		m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		(INT)m_CurrentBackBufferIdx,
		m_RTVSize
	};
}

D3D12_CPU_DESCRIPTOR_HANDLE WindowsWindow::GetDepthStencilView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE{
	m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
	(INT)m_CurrentBackBufferIdx,
	m_DSVSize
	};
}

void WindowsWindow::Present(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList,bool shouldVSync)
{
	TransitionResource(directCommandList, m_BackBufferResource[m_CurrentBackBufferIdx],
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	const UINT syncInterval{ shouldVSync };
	const UINT presentFlags{ m_IsTearingSupported && !shouldVSync ? DXGI_PRESENT_ALLOW_TEARING : 0u };
	
	const auto directCommandQueue{
	Instance<DX12Resource>().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT) };

	auto& currentFenceValue{ m_FenceValues[m_CurrentBackBufferIdx] };
	
	currentFenceValue = directCommandQueue->ExecuteCommandList(directCommandList);
	
	ThrowIfFailed(m_WindowSwapChain->Present(syncInterval, presentFlags));
	m_CurrentBackBufferIdx = m_WindowSwapChain->GetCurrentBackBufferIndex();

	directCommandQueue->WaitForFenceValue(currentFenceValue);
}

void WindowsWindow::ClearBackBuffer(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList,
                                     const float(&clearColor)[4]) const
{
	TransitionResource(directCommandList, m_BackBufferResource[m_CurrentBackBufferIdx],
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv{
		m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		(INT)m_CurrentBackBufferIdx,
		m_RTVSize
	};

	directCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

	ID3D12DescriptorHeap* descriptor[]{
		m_GPUDescriptorHeap[m_CurrentBackBufferIdx].Get()
	};
	
	directCommandList->SetDescriptorHeaps(_countof(descriptor), descriptor);
}

void WindowsWindow::UpdateRenderTargetViews(const ComPtr<ID3D12Device2>& device)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv{
		m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	};

	// Create Render Target View
	for (int i = 0; i < BackBufferCnt; ++i)
	{
		ComPtr<ID3D12Resource> backBuffer{};
		ThrowIfFailed(m_WindowSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv);

		m_BackBufferResource[i] = backBuffer;

		rtv.Offset(m_RTVSize);
	}
	
}

void WindowsWindow::UpdateDepthStencilView(const ComPtr<ID3D12Device2>& device, UINT width, UINT height)
{
	// Flush GPU commands that referencing depth buffer
	Instance<DX12Resource>().FlushCommands();
		
	// Create Depth Buffer
	CD3DX12_HEAP_PROPERTIES heapProps{ D3D12_HEAP_TYPE_DEFAULT };
	CD3DX12_RESOURCE_DESC textureDesc{ CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		width,height,
		1,0,1,0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) };

	D3D12_CLEAR_VALUE optimizedClearValue{};
	
	ThrowIfFailed(
		device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&optimizedClearValue,
			IID_PPV_ARGS(&m_DepthBuffer)));

	// Update Depth Stencil View
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv{};
	dsv.Format = DXGI_FORMAT_D32_FLOAT;
	dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsv.Texture2D.MipSlice = 0;
	dsv.Flags = D3D12_DSV_FLAG_NONE;

	device->CreateDepthStencilView(
		m_DepthBuffer.Get(),
		&dsv,
		m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	
	m_CPUDescriptorHeap = CreateDescriptorHeap(device,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, m_DescriptorCnt);

	for (int i = 0; i < BackBufferCnt; ++i)
	{
		m_GPUDescriptorHeap[i] = CreateDescriptorHeap(device,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			m_DescriptorCnt);
	}
}

bool WindowsWindow::CheckTearingSupport()
{
	BOOL allowTearing{};

	// Rather than create the DXGI 1.5 factory interface directly, we create the
	// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
	// graphics debugging tools which will not support the 1.5 factory interface 
	// until a future update.
	ComPtr<IDXGIFactory4> factory4;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
	{
		ComPtr<IDXGIFactory5> factory5;
		if (SUCCEEDED(factory4.As(&factory5)))
		{
			if (FAILED(factory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing, sizeof(allowTearing))))
			{
				allowTearing = FALSE;
			}
		}
	}

	return allowTearing == TRUE;
}

void WindowsWindow::TransitionResource(const ComPtr<ID3D12GraphicsCommandList2>& commandList,
	const ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) const
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		resource.Get(),
		beforeState, afterState);

	commandList->ResourceBarrier(1, &barrier);
}

ComPtr<IDXGISwapChain4> WindowsWindow::CreateSwapChain(
	const ComPtr<ID3D12CommandQueue>& directCommandQueue,
	HWND windowHandle, 
	UINT width, 
	UINT height)
{
	ComPtr<IDXGISwapChain4> dxgiSwapChain4{};
	ComPtr<IDXGIFactory4> dxgiFactory4{};
	UINT createFactoryFlags{};
#ifdef _DEBUG
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(::CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = BackBufferCnt;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// It is recommended to always allow tearing if tearing support is available.
	swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> swapChain1{};

	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(
		directCommandQueue.Get(),
		windowHandle,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1));

	// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
	// will be handled manually.
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	return dxgiSwapChain4;
}

ComPtr<ID3D12DescriptorHeap> WindowsWindow::CreateDescriptorHeap(
	const ComPtr<ID3D12Device2>& device,
	D3D12_DESCRIPTOR_HEAP_FLAGS flags,
	D3D12_DESCRIPTOR_HEAP_TYPE type,
	UINT numDescriptor) const
{
	ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = flags;
	desc.Type = type;
	desc.NumDescriptors = numDescriptor;

	ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap)));

	return descHeap;
}

#endif