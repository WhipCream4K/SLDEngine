#ifndef SLDFRAMEWORK_WINDOWSUBSYSTEM_H
#define SLDFRAMEWORK_WINDOWSUBSYSTEM_H

#include "../../Miscellaneous/WinPrerequisite.h"
#include "../../../Rendering/DX12/DX3D12Prerequisite.h"

#if defined(_WIN32) && defined (HAS_DX12)

// OS specific window
class WindowsWindow
{

public:

	WindowsWindow(
		HWND windowHandle,
		uint32_t width, uint32_t height);

	const ComPtr<ID3D12Resource> GetBackBufferResource(UINT backBufferIdx) const;
	const ComPtr<IDXGISwapChain4> GetSwapChain() const;

	UINT GetCurrentBackBufferIndex() const { return m_CurrentBackBufferIdx; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

	void Present(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList, bool shouldVSync);
	void ClearBackBuffer(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList, const float(&clearColor)[4]) const;

	void UpdateRenderTargetViews(const ComPtr<ID3D12Device2>& device);
	void UpdateDepthStencilView(const ComPtr<ID3D12Device2>& device, UINT width, UINT height);

	static constexpr UINT8 BackBufferCnt{ 3 };

private:

	bool CheckTearingSupport();

	void TransitionResource(
		const ComPtr<ID3D12GraphicsCommandList2>& commandList,
		const ComPtr<ID3D12Resource>& resource,
		D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) const;

	ComPtr<IDXGISwapChain4> CreateSwapChain(
		const ComPtr<ID3D12CommandQueue>& directCommandQueue,
		HWND windowHandle, UINT width, UINT height);

	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		const ComPtr<ID3D12Device2>& device,
		D3D12_DESCRIPTOR_HEAP_FLAGS flags,
		D3D12_DESCRIPTOR_HEAP_TYPE type,
		UINT numDescriptor) const;


	static constexpr UINT32 m_DescriptorCnt{ 2048 };

	ComPtr<IDXGISwapChain4> m_WindowSwapChain{};
	ComPtr<ID3D12Resource> m_BackBufferResource[BackBufferCnt]{};
	UINT64 m_FenceValues[BackBufferCnt]{};

	ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap{};
	UINT m_RTVSize{};

	ComPtr<ID3D12DescriptorHeap> m_DSVDescriptorHeap{};
	ComPtr<ID3D12Resource> m_DepthBuffer{};
	UINT m_DSVSize{};

	ComPtr<ID3D12DescriptorHeap> m_GPUDescriptorHeap[BackBufferCnt]{}; // Shader Visible
	ComPtr<ID3D12DescriptorHeap> m_CPUDescriptorHeap{}; // Non-Shader Visible
	

	UINT m_CurrentBackBufferIdx{};
	bool m_IsTearingSupported{};
};

#endif

#endif