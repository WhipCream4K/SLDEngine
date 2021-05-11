#include "Renderer.h"

#include "../Core/Window/Window.h"
#include "../Components/RenderingComponent.h"

void SLD::Renderer::Render(const RefPtr<Window>& renderWindow, 
                           const std::vector<RefPtr<RenderingComponent>>& renderingComponents)
{
	auto callRender{ [&renderWindow,&renderingComponents](auto& subsystem) -> void
	{
		subsystem.SetRenderTarget(renderWindow);
		subsystem.Render(renderingComponents);
	}
	};

	std::visit(callRender, m_SubSystemRenderer);
}

void SLD::Renderer::Render(const RefPtr<Window>& renderWindow,
	const std::vector<RenderingComponent>& renderingComponents)
{
	auto callRender{ [&renderWindow,&renderingComponents](auto& subsystem) -> void
{
	subsystem.SetRenderTarget(renderWindow);
	subsystem.Render(renderingComponents);
}
	};

	std::visit(callRender, m_SubSystemRenderer);
}
