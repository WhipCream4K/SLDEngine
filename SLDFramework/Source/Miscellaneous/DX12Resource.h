#ifndef DX12RESOURCE_H
#define DX12RESOURCE_H

#include "WinPrerequisite.h"
#include "../Rendering/DX12/DX3D12Prerequisite.h"
#include "../Rendering/DX12/DX12CommandQueue.h"

#include "../Core/Base.h"

#ifdef HAS_DX12

class DX12Resource final
{
public:
	
	static DX12Resource& Instance()
	{
		static DX12Resource instance{};
		return instance;
	}

	void Init(bool shouldUseWARP);
	const ComPtr<ID3D12Device2>& GetDevice() const { return m_MainDevice; }
	SharedPtr<DX12CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type) const;
	void FlushCommands();

private:

	ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	ComPtr<ID3D12Device2> CreateDevice(const ComPtr<IDXGIAdapter4>& dxgiAdapter);
	void EnableDebugLayer();
	
	DX12Resource() = default;

private:

	ComPtr<ID3D12Device2> m_MainDevice;
	SharedPtr<DX12CommandQueue> m_DirectCommandQueue;
	SharedPtr<DX12CommandQueue> m_CopyCommandQueue;
	SharedPtr<DX12CommandQueue> m_ComputeCommandQueue;
};

#endif

#endif


