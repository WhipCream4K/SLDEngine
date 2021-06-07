#include "Renderer.h"

#include "../Core/Window/Window.h"
#include "../Components/RenderingComponent.h"

//void SLD::Renderer::Render(const std::vector<RefPtr<RenderingComponent>>& renderingComponents)
//{
//	auto callRender{ [&renderingComponents](auto& subsystem) -> void
//	{
//		subsystem.Render(renderingComponents);
//	} };
//
//	std::visit(callRender, m_SubSystemRenderer);
//}

void SLD::Renderer::Render(uint32_t elemCnt, uint8_t* bufferHead, size_t bufferSize)
{
	auto callRender{ [&elemCnt,&bufferHead,&bufferSize](auto& subSystem)
	{
		subSystem.Render(elemCnt,bufferHead,bufferSize);
	} };

	std::visit(callRender, m_SubSystemRenderer);
}

//void SLD::Renderer::Render(const std::vector<RenderingComponent>& renderingComponents)
//{
//	auto callRender{ [&renderingComponents](auto& subsystem) -> void
//	{
//		subsystem.Render(renderingComponents);
//	} };
//
//	std::visit(callRender, m_SubSystemRenderer);
//}

//void SLD::Renderer::Render(std::vector<RenderingComponent>& renderingComponents)
//{
//	auto callRender{ [&renderingComponents](auto& subsystem) -> void
//{
//	subsystem.Render(renderingComponents);
//} };
//
//	std::visit(callRender, m_SubSystemRenderer);
//}

void SLD::Renderer::SetRenderWindow(const RefPtr<Window>& renderWindow)
{
	auto callSetRenderTarget{ [&renderWindow](auto& subSystem) -> void
	{
		subSystem.SetRenderTarget(renderWindow);
	} };

	std::visit(callSetRenderTarget, m_SubSystemRenderer);
}

void SLD::Renderer::SetRenderWindow(Window& renderWindow)
{
	auto callSetRenderTarget{ [&renderWindow](auto& subSystem) -> void
{
	subSystem.SetRenderTarget(renderWindow);
} };

	std::visit(callSetRenderTarget, m_SubSystemRenderer);
}
