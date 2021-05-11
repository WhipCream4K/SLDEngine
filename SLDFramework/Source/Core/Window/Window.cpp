#include "Window.h"
#include "../../Miscellaneous/DX12Resource.h"


SLD::Window::Window(uint32_t width, uint32_t height, LLWindow&& subSystemWindow, LLInputs&& subSystemInput,
	const CurrentWindow& windowHandle, const std::string& vpName)
	: m_InputManager(std::move(subSystemInput))
	, m_WindowSubSystem(std::move(subSystemWindow))
	, m_Name(vpName)
	, m_Height(height)
	, m_Width(width)
	, m_WindowHandle(windowHandle)
	, m_ClearColor{}
	, m_ShouldVSync()
{
}

void SLD::Window::Resize(uint32_t width, uint32_t height)
{
#ifdef HAS_DX12

	const auto device{ Instance<DX12Resource>().GetDevice() };

	m_WindowSubSystem.UpdateRenderTargetViews(device);
	m_WindowSubSystem.UpdateDepthStencilView(device, width, height);

	m_Width = width;
	m_Height = height;

#endif

	auto callResize{ [&width,&height](auto& window)
	{
		window.Resize(width,height);
	} };

	std::visit(callResize, m_WindowSubSystem);
	
}

void SLD::Window::Present()
{
#ifdef HAS_DX12

	const auto directCommandQueue{
	Instance<DX12Resource>().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT) };

	const auto directCommandList{ directCommandQueue->GetCommandList() };

	m_WindowSubSystem.Present(directCommandList, m_ShouldVSync);

#endif

	auto callPresent{ [this](auto& window)
	{
		window.Present(m_ShouldVSync);
	} };

	std::visit(callPresent, m_WindowSubSystem);
}

void SLD::Window::ClearBackBuffer()
{
#ifdef HAS_DX12

	const auto directCommandQueue{
	Instance<DX12Resource>().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT) };

	const auto directCommandList{ directCommandQueue->GetCommandList() };

	m_WindowSubSystem.ClearBackBuffer(directCommandList, m_ClearColor);

#endif

	auto callClearBackBuffer{ [this](auto& window)
	{
		window.ClearColor(m_ClearColor);
	} };

	std::visit(callClearBackBuffer, m_WindowSubSystem);
}


