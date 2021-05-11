
#ifndef SLDFRAMEWORK_GUIDEBUGGER_H
#define SLDFRAMEWORK_GUIDEBUGGER_H

#include "../Core/Base.h"
#include "../Components/RenderingComponent.h"

#include "../Miscellaneous/WinPrerequisite.h"
#include "../Rendering/DX12/DX3D12Prerequisite.h"

#include "../Core/Window/Window.h"

struct ImGuiIO;
struct ImGuiContext;

namespace SLD
{
	class Renderer;
	class Window;

	// TODO: Update to support multiple windows
	class GUIDebugComponent final
	{
	public:

		GUIDebugComponent();
		bool AttachDrawWindow(const RefPtr<Window>& drawWindow);

		// Dangerous; no runtime null checking here, be sure to attach the viewport before call this function
		void NewImmediateFrame(float dt);

		[[nodiscard]] RenderingComponent GetImmedateRenderComponent() const;

	private:

		static void SetUpImGuiKeyMap(ImGuiIO& io);
		void SetUpImGuiFont(ImGuiIO& io);

	private:

		//static constexpr uint8_t m_MinimumBackBufferCnt{ 3 };

		RefPtr<ImGuiContext> m_ImGuiContext{};
		WeakPtr<Window> m_DrawWindow;
		bool m_IsInitialized{};

		
		// TODO: Decouple these elements
#ifdef HAS_DX12

		struct PerFrameResource
		{
			ComPtr<ID3D12Resource> vertexBuffer{};
			uint32_t vertexCnt{};
			ComPtr<ID3D12Resource> indexBuffer{};
			uint32_t indexCnt{};
		};
		
		ComPtr<ID3D12PipelineState> SetUpPipelineStateObject(const ComPtr<ID3D12Device2>& device);
		ComPtr<ID3D12RootSignature> SetupRootSignature(const ComPtr<ID3D12Device2>& device);
		void						SetupFrameResources(PerFrameResource& out,const ComPtr<ID3D12Device2>& device);
		


		ComPtr<ID3D12PipelineState> m_PipeLineStateObject;
		ComPtr<ID3D12RootSignature> m_RootSignature;

		ComPtr<ID3D12Resource> m_FontResource;
		D3D12_CPU_DESCRIPTOR_HANDLE m_FontDescriptorHandle{};

		PerFrameResource m_FrameResources[SFMLWindow::BackBufferCnt]{};
		
#endif

	};
}


#endif
