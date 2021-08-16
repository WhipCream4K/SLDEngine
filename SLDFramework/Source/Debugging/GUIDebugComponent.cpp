#include "GUIDebugComponent.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "../Miscellaneous/DX12Resource.h"

SLD::GUIDebugComponent::GUIDebugComponent()
{
	IMGUI_CHECKVERSION();

	m_ImGuiContext = SharedPtr<ImGuiContext>{ ImGui::CreateContext(),[](ImGuiContext* context)
	{
		ImGui::DestroyContext(context);
		context = nullptr;
	} };

	ImGui::SetCurrentContext(m_ImGuiContext.get());
	ImGui::StyleColorsDark(&m_ImGuiContext->Style);

	const int backendFlags = ImGuiBackendFlags_HasGamepad |
		ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_RendererHasVtxOffset;

	ImGuiIO& imGuiIO = m_ImGuiContext->IO;

	imGuiIO.BackendFlags = backendFlags;

	SetUpImGuiKeyMap(imGuiIO);
	SetUpImGuiFont(imGuiIO);

#ifdef HAS_DX12

	const auto& device = Instance<DX12Resource>().GetDevice();

	m_PipeLineStateObject = SetUpPipelineStateObject(device);
	m_RootSignature = SetupRootSignature(device);

	for (int i = 0; i < SFMLWindow::BackBufferCnt; ++i)
	{
		SetupFrameResources(m_FrameResources[i], device);
	}

#endif
}

bool SLD::GUIDebugComponent::AttachDrawWindow(const SharedPtr<Window>& drawWindow)
{
	if (drawWindow)
	{
		m_DrawWindow = drawWindow;

		//// setup ImGui context
		//// NOTE: THIS VERSION OF ImGui IS NOT THREAD-SAFE
		//IMGUI_CHECKVERSION();

		//m_ImGuiContext = SharedPtr<ImGuiContext>{ ImGui::CreateContext(),[](ImGuiContext* context)
		//{
		//	ImGui::DestroyContext(context);
		//	context = nullptr;
		//} };

		//ImGui::SetCurrentContext(m_ImGuiContext.get());

		//ImGui::StyleColorsDark(&m_ImGuiContext->Style); // default dark just like everything should be :)

		//// setup backend capabilities flags
		//const int backendFlags = ImGuiBackendFlags_HasGamepad |
		//	ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_RendererHasVtxOffset;

		ImGuiIO& imGuiIO = m_ImGuiContext->IO;

		imGuiIO.BackendPlatformName = drawWindow->GetTitleName().c_str();
		imGuiIO.ImeWindowHandle = drawWindow->GetNativeWindowHandle().get();

		return true;
	}

	return false;
}

void SLD::GUIDebugComponent::NewImmediateFrame(float dt)
{
	ImGuiIO& io{ m_ImGuiContext->IO };
	const auto currWindow{ m_DrawWindow.lock() };

	// Setup display size for every frame to accomodate window resizing
	io.DisplaySize = ImVec2{ float(currWindow->GetWidth()),float(currWindow->GetHeight()) };

	// Setup time step
	io.DeltaTime = dt;

	// Update user events
	//std::array<>
	//const EventQueueHandle inputsFromViewPort{};

	//for (uint8_t i = 0; i < inputsFromViewPort.eventCntThisFrame; ++i)
	//{
	//	const auto& message{ inputsFromViewPort.events[i] };

	//	switch (message.eventId)
	//	{
	//	case EventType::KeyPressed:

	//		io.KeysDown[message.data.keyboard.key] = true;
	//		io.KeyCtrl = message.data.keyboard.ctrl;
	//		io.KeyAlt = message.data.keyboard.alt;
	//		io.KeyShift = message.data.keyboard.shift;
	//		io.KeySuper = message.data.keyboard.system;

	//		break;

	//	case EventType::KeyReleased:

	//		io.KeysDown[message.data.keyboard.key] = false;
	//		io.KeyCtrl = message.data.keyboard.ctrl;
	//		io.KeyAlt = message.data.keyboard.alt;
	//		io.KeyShift = message.data.keyboard.shift;
	//		io.KeySuper = message.data.keyboard.system;

	//		break;
	//	case EventType::MouseButtonPressed:		io.MouseDown[int(message.data.mouse.key)] = true;								break;
	//	case EventType::MouseButtonReleased:	io.MouseDown[int(message.data.mouse.key)] = false;								break;
	//	case EventType::MouseWheelScrolled:		io.MouseWheel = message.data.mouse.verticalScroll;								break;
	//	case EventType::MouseMoved:				io.MousePos = { message.data.mouse.x,message.data.mouse.y };					break;
	//	default:																												break;
	//	}
	//}

	//// TODO: Probably also update the gamepad as well

	//// TODO: Create Immediate Rendering Commponent

	//// External ImGui
	//ImGui::NewFrame();
}

//SLD::RenderingComponent SLD::GUIDebugComponent::GetImmedateRenderComponent() const
//{
//	ImGui::Render();
//	//RenderingComponent out{ 10,10 };
//	return out;
//}

void SLD::GUIDebugComponent::SetUpImGuiKeyMap(ImGuiIO& io)
{
	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
	io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
	io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Enter;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_KeyPadEnter] = sf::Keyboard::Return;

	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;
}

void SLD::GUIDebugComponent::SetUpImGuiFont(ImGuiIO& io)
{
	uint8_t* pixels{};
	int width{}, height{};

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

#ifdef HAS_DX12

	auto dx12Resource{ Instance<DX12Resource>() };
	const auto& device{ dx12Resource.GetDevice() };
	const auto directCommandQueue{ dx12Resource.GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT) };
	const auto directCommandList{ directCommandQueue->GetCommandList() };

	// Upload texture to graphics system
	{
		CD3DX12_HEAP_PROPERTIES heapProps{ D3D12_HEAP_TYPE_DEFAULT };
		CD3DX12_RESOURCE_DESC bufferDesc{ CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,width,height,1,1) };

		device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr, IID_PPV_ARGS(&m_FontResource));

		// Flatten texture2D into 1D buffer
		UINT uploadPitch = (width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
		UINT uploadSize = height * uploadPitch;

		bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadSize);
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		ComPtr<ID3D12Resource> uploadBuffer{};

		device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadBuffer)
		);

		// Copy texture data from CPU to GPU
		void* mapped = NULL;
		D3D12_RANGE range = { 0, uploadSize };
		ThrowIfFailed(uploadBuffer->Map(0, &range, &mapped));
		for (int y = 0; y < height; y++)
			memcpy((void*)((uintptr_t)mapped + y * uploadPitch), pixels + y * width * 4, width * 4);
		uploadBuffer->Unmap(0, &range);

		// Use CopyTextureRegion from MSN documentation
		// https://docs.microsoft.com/en-us/windows/win32/direct3d12/upload-and-readback-of-texture-data

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedSubResc{};

		device->GetCopyableFootprints(
			&bufferDesc, 0, 1, 0,
			&placedSubResc,
			nullptr,
			nullptr,
			nullptr);

		CD3DX12_TEXTURE_COPY_LOCATION dest{ m_FontResource.Get() };
		CD3DX12_TEXTURE_COPY_LOCATION src{ uploadBuffer.Get(),placedSubResc };

		directCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

		// Turn Buffer Resource to Shader Resource
		CD3DX12_RESOURCE_BARRIER rescBarrier{
			CD3DX12_RESOURCE_BARRIER::Transition(m_FontResource.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) };

		directCommandList->ResourceBarrier(1, &rescBarrier);

		// Execute commands
		uint64_t fenceValue{ directCommandQueue->ExecuteCommandList(directCommandList) };
		directCommandQueue->WaitForFenceValue(fenceValue);

		// Create Texture view
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = bufferDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		device->CreateShaderResourceView(m_FontResource.Get(), &srvDesc, m_FontDescriptorHandle);
	}

	// Store identifier
	//static_assert(sizeof(ImTextureID) >= sizeof(g_hFontSrvGpuDescHandle.ptr), "Can't pack descriptor handle into TexID, 32-bit not supported yet.");
	io.Fonts->SetTexID((ImTextureID)m_FontDescriptorHandle.ptr);

	//device->CopyDescriptors()

#endif
}

#ifdef HAS_DX12

ComPtr<ID3D12PipelineState> SLD::GUIDebugComponent::SetUpPipelineStateObject(const ComPtr<ID3D12Device2>& device)
{

#pragma region vertexShader

	static const char* vertexShader =
		"cbuffer vertexBuffer : register(b0) \
            {\
              float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
              float2 pos : POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
              PS_INPUT output;\
              output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
              output.col = input.col;\
              output.uv  = input.uv;\
              return output;\
            }";

#pragma endregion

#pragma region pixelShader

	static const char* pixelShader =
		"struct PS_INPUT\
            {\
              float4 pos : SV_POSITION;\
              float4 col : COLOR0;\
              float2 uv  : TEXCOORD0;\
            };\
            SamplerState sampler0 : register(s0);\
            Texture2D texture0 : register(t0);\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
              float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
              return out_col; \
            }";

#pragma endregion 

	ComPtr<ID3D12PipelineState> pipelineStateObject;

	struct PipelineStateStream
	{
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendDesc;
		CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER Rasterizer;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DepthStencil;

	}pipelineStateStream;

	// Create Vertex Shader
	{
		ComPtr<ID3DBlob> vertexShaderBlob;
		if (FAILED(D3DCompile(vertexShader, strlen(vertexShader), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &vertexShaderBlob, NULL)))
			return pipelineStateObject;

		D3D12_INPUT_ELEMENT_DESC local_layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, uv),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)IM_OFFSETOF(ImDrawVert, col), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		pipelineStateStream.InputLayout = { local_layout,_countof(local_layout) };
		pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE{ vertexShaderBlob.Get() };
	}

	// Create Pixel Shader
	{
		ComPtr<ID3DBlob> pixelShaderBlob{};
		if (FAILED(D3DCompile(pixelShader, strlen(pixelShader), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pixelShaderBlob, NULL)))
			return pipelineStateObject;

		pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE{ pixelShaderBlob.Get() };
	}

	// Create Root Signature
	{
		pipelineStateStream.pRootSignature = m_RootSignature.Get();
	}

	// Create Primitive Topology
	{
		pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}

	// Create DSV format
	{
		pipelineStateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}

	// Create RTV format
	{
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateStream.RTVFormats = rtvFormats;
	}

	// Create Blend Desc
	{
		CD3DX12_BLEND_DESC desc{};
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		pipelineStateStream.BlendDesc = desc;
	}

	// Create Rasterizer state
	{
		CD3DX12_RASTERIZER_DESC desc{};
		desc.FillMode = D3D12_FILL_MODE_SOLID;
		desc.CullMode = D3D12_CULL_MODE_NONE;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = FALSE;
		desc.AntialiasedLineEnable = FALSE;
		desc.ForcedSampleCount = 0;
		desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		pipelineStateStream.Rasterizer = desc;
	}

	// Create Depth-Stencil State
	{
		CD3DX12_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		desc.BackFace = desc.FrontFace;

		pipelineStateStream.DepthStencil = desc;
	}

	D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
	sizeof(PipelineStateStream), &pipelineStateStream
	};

	ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&pipelineStateObject)));

	return pipelineStateObject;
}

ComPtr<ID3D12RootSignature> SLD::GUIDebugComponent::SetupRootSignature(const ComPtr<ID3D12Device2>& device)
{

	ComPtr<ID3D12RootSignature> rootSignature{};

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	if (FAILED(device->CheckFeatureSupport(
		D3D12_FEATURE_ROOT_SIGNATURE,
		&featureData,
		sizeof(featureData))))
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags{
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
	};

	CD3DX12_ROOT_PARAMETER1 rootParameters[2]{};

	rootParameters[0].InitAsConstants(16, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	D3D12_DESCRIPTOR_RANGE1 descriptorRange{};
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RegisterSpace = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = 0;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;

	rootParameters[0].InitAsDescriptorTable(0, &descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);

	CD3DX12_STATIC_SAMPLER_DESC staticSamplerDisc{};

	staticSamplerDisc.Init(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,

		0.0f, 0.0f,
		D3D12_COMPARISON_FUNC_ALWAYS,
		D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
		0.0f, 0.0f, D3D12_SHADER_VISIBILITY_PIXEL);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription{};
	rootSignatureDescription.Init_1_1(
		_countof(rootParameters), rootParameters,
		1, &staticSamplerDisc, rootSignatureFlags);

	ComPtr<ID3DBlob> rootSignatureBlob{};
	ComPtr<ID3DBlob> errorBlob{};

	ThrowIfFailed(::D3DX12SerializeVersionedRootSignature(
		&rootSignatureDescription, featureData.HighestVersion, &rootSignatureBlob, &errorBlob));

	ThrowIfFailed(device->CreateRootSignature(0,
		rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)));

	return rootSignature;
}

void SLD::GUIDebugComponent::SetupFrameResources(PerFrameResource& out, const ComPtr<ID3D12Device2>& device)
{
	out.indexCnt = 10000;
	out.vertexCnt = 5000;

	// Create Index Buffer
	D3D12_HEAP_PROPERTIES heapProps{ D3D12_HEAP_TYPE_DEFAULT };
	CD3DX12_RESOURCE_DESC bufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(ImDrawIdx) * out.indexCnt) };

	device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&out.indexBuffer));

	// Create Vertex Buffer
	bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ImDrawVert) * out.vertexCnt);

	device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&out.vertexBuffer));

}

#endif
