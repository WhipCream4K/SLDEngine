#ifndef SLDFRAMEWORK_DX12COMMAND_H
#define SLDFRAMEWORK_DX12COMMAND_H

#include "../../Miscellaneous/WinPrerequisite.h"
#include "DX3D12Prerequisite.h"

#include <queue>

#ifdef HAS_DX12

// Thanks to jpvanoosten to provide this CommandQueue and help with DirectX12
// https://github.com/jpvanoosten/LearningDirectX12/tree/v0.0.2

class DX12CommandQueue final
{
public:
	
    DX12CommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);

    // Get an available command list from the command queue.
    ComPtr<ID3D12GraphicsCommandList2> GetCommandList();

    // Execute a command list.
    // Returns the fence value to wait for for this command list.
    uint64_t ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList);

    uint64_t Signal();
    bool IsFenceComplete(uint64_t fenceValue);
    void WaitForFenceValue(uint64_t fenceValue);
    void Flush();

    ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const;

    ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
    ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator);

private:
	
    // Keep track of command allocators that are "in-flight"
    struct CommandAllocatorEntry
    {
        uint64_t fenceValue;
        ComPtr<ID3D12CommandAllocator> commandAllocator;
    };

    using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
    using CommandListQueue = std::queue<ComPtr<ID3D12GraphicsCommandList2> >;

    D3D12_COMMAND_LIST_TYPE                     m_CommandListType;
    ComPtr<ID3D12Device2>       m_d3d12Device;
    ComPtr<ID3D12CommandQueue>  m_d3d12CommandQueue;
    ComPtr<ID3D12Fence>         m_d3d12Fence;
    HANDLE                                      m_FenceEvent;
    uint64_t                                    m_FenceValue;

    CommandAllocatorQueue                       m_CommandAllocatorQueue;
    CommandListQueue                            m_CommandListQueue;
};

#endif

#endif