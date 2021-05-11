#ifndef SLDFRAMEWORK_DX12_RENDERER_H
#define SLDFRAMEWORK_DX12_RENDERER_H

#include "../../Miscellaneous/WinPrerequisite.h"
#include "DX3D12Prerequisite.h"
#include "../../Core/Base.h"


namespace SLD
{
	class RenderingComponent;
	class Window;
}

#if defined(WIN32) && defined(HAS_DX12)
// encapsulate dx12 renderer
class DX12Renderer final
{
public:
	
	void Render(const std::vector<RefPtr<SLD::RenderingComponent>>& renderingComponents) const;

	void Render(const std::vector<SLD::RenderingComponent>& renderingComponents) const;

	//void SetRenderTarget(
	//	const ComPtr<ID3D12GraphicsCommandList2>& directCommandList,
	//	D3D12_CPU_DESCRIPTOR_HANDLE rtvPos,
	//	D3D12_CPU_DESCRIPTOR_HANDLE dsvPos = {}) const;

	void SetRenderTarget(const SLD::Window& renderWindow);
	void SetRenderTarget(const RefPtr<SLD::Window>& renderWindow);

private:

	//void TransitionResource(
	//	const ComPtr<ID3D12GraphicsCommandList2>& commandList,
	//	Microsoft::WRL::ComPtr<ID3D12Resource> resource,
	//	D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) const;
};

#endif
#endif
