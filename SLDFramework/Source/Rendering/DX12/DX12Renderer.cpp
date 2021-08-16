#include "DX12Renderer.h"
#include "../../Components/RenderingComponent.h"

#ifdef HAS_DX12

void DX12Renderer::Render(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList,
	const std::vector<SharedPtr<SLD::RenderingComponent>>& renderingComponents) const
{
	// assume that render target is already in RENDER_TARGET state
	// *** RENDER CODE HERE ***
}

void DX12Renderer::Render(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList,
	const std::vector<SLD::RenderingComponent>& renderingComponents) const
{
	// assume that render target is already in RENDER_TARGET state
	// *** RENDER CODE HERE ***
}

void DX12Renderer::SetRenderTarget(const ComPtr<ID3D12GraphicsCommandList2>& directCommandList, 
                                   D3D12_CPU_DESCRIPTOR_HANDLE rtvPos,
                                   D3D12_CPU_DESCRIPTOR_HANDLE dsvPos) const
{
	// Assume that the Render Target is already in RENDER_TARGET state

	directCommandList->OMSetRenderTargets(
		1, 
		&rtvPos, 
		false,
		&dsvPos);
}

void DX12Renderer::TransitionResource(const ComPtr<ID3D12GraphicsCommandList2>& commandList,
	Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState,
	D3D12_RESOURCE_STATES afterState) const
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		resource.Get(),
		beforeState, afterState);

	commandList->ResourceBarrier(1, &barrier);
}

#endif

//void DX12Renderer::Render(const std::vector<SharedPtr<SLD::RenderingComponent>>& renderingComponents) const
//{
//	renderingComponents;
//}
//
//void DX12Renderer::Render(const std::vector<SLD::RenderingComponent>& renderingComponents) const
//{
//	renderingComponents;
//}
//
//void DX12Renderer::SetRenderTarget(const SLD::Window& renderWindow)
//{
//	renderWindow;
//}
//
//void DX12Renderer::SetRenderTarget(const SharedPtr<SLD::Window>& renderWindow)
//{
//	renderWindow;
//}
