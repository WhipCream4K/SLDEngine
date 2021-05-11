#include "DX12CommandQueue.h"

#ifdef HAS_DX12

DX12CommandQueue::DX12CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
	: m_CommandListType(type)
	, m_d3d12Device(device)
	, m_d3d12CommandQueue()
	, m_d3d12Fence()
	, m_FenceEvent()
	, m_FenceValue()
	, m_CommandAllocatorQueue()
	, m_CommandListQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
	ThrowIfFailed(m_d3d12Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));

	m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	//assert(m_FenceEvent && "Failed to create fence event handle.");
}

ComPtr<ID3D12GraphicsCommandList2> DX12CommandQueue::GetCommandList()
{
	ComPtr<ID3D12CommandAllocator> commandAlloc{};
	ComPtr<ID3D12GraphicsCommandList> commandList{};

    if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue))
    {
        commandAlloc = m_CommandAllocatorQueue.front().commandAllocator;
        m_CommandAllocatorQueue.pop();

        ThrowIfFailed(commandAlloc->Reset());
    }
    else
    {
        commandAlloc = CreateCommandAllocator();
    }

    if (!m_CommandListQueue.empty())
    {
        commandList = m_CommandListQueue.front();
        m_CommandListQueue.pop();

        ThrowIfFailed(commandList->Reset(commandAlloc.Get(), nullptr));
    }
    else
    {
        commandList = CreateCommandList(commandAlloc);
    }

    // Associate the command allocator with the command list so that it can be
    // retrieved when the command list is executed.
    ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAlloc.Get()));
}

uint64_t DX12CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList)
{
    commandList->Close();

    ID3D12CommandAllocator* commandAllocator;
    UINT dataSize = sizeof(commandAllocator);
    ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

    ID3D12CommandList* const ppCommandLists[] = {
        commandList.Get()
    };

    m_d3d12CommandQueue->ExecuteCommandLists(1, ppCommandLists);
    uint64_t fenceValue = Signal();

    m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
    m_CommandListQueue.push(commandList);

    // The ownership of the command allocator has been transferred to the ComPtr
    // in the command allocator queue. It is safe to release the reference 
    // in this temporary COM pointer here.
    commandAllocator->Release();

    return fenceValue;
}

uint64_t DX12CommandQueue::Signal()
{
    uint64_t fenceValue = ++m_FenceValue;
    m_d3d12CommandQueue->Signal(m_d3d12Fence.Get(), fenceValue);
    return fenceValue;
}

bool DX12CommandQueue::IsFenceComplete(uint64_t fenceValue)
{
    return m_d3d12Fence->GetCompletedValue() >= fenceValue;
}

void DX12CommandQueue::WaitForFenceValue(uint64_t fenceValue)
{
    if (!IsFenceComplete(fenceValue))
    {
        m_d3d12Fence->SetEventOnCompletion(fenceValue, m_FenceEvent);
        ::WaitForSingleObject(m_FenceEvent, DWORD_MAX);
    }
}

void DX12CommandQueue::Flush()
{
    WaitForFenceValue(Signal());
}

ComPtr<ID3D12CommandQueue> DX12CommandQueue::GetD3D12CommandQueue() const
{
    return m_d3d12CommandQueue;
}

ComPtr<ID3D12CommandAllocator> DX12CommandQueue::CreateCommandAllocator()
{
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    ThrowIfFailed(m_d3d12Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));

    return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList2> DX12CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator)
{
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;
    ThrowIfFailed(m_d3d12Device->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

    return commandList;
}

#endif
