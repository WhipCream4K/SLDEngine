#include "SFMLRenderer.h"

#include "../../Core/Window/Window.h"

void SFMLRenderer::Render(const std::vector<SLD::RenderingComponent>& renderingComponents)
{
	//m_RenderWindow.draw();
	renderingComponents;
}

void SFMLRenderer::Render(const std::vector<RefPtr<SLD::RenderingComponent>>& renderingComponents)
{
	renderingComponents;
	//renderingComponents;
}

void SFMLRenderer::SetRenderTarget(const SLD::Window& renderWindow)
{
	//sf::WindowHandle newWindowHandle{ renderWindow.GetWindowHandleToType<sf::WindowHandle>() };
	//if(m_RenderWindow->getSystemHandle() != newWindowHandle)
	//{
	//	m_RenderWindow.create(newWindowHandle);
	//}
	renderWindow;
}

void SFMLRenderer::SetRenderTarget(const RefPtr<SLD::Window>& renderWindow)
{
	auto windowSubSystem{ renderWindow->GetWindowSubsystem<SFMLWindow>()->GetSubRefPtr() };
	if(windowSubSystem != m_RenderWindow)
	{
		m_RenderWindow = windowSubSystem;
	}
	//sf::WindowHandle newWindowHandle{ renderWindow->GetWindowHandleToType<sf::WindowHandle>() };
	//if (m_RenderWindow->getSystemHandle() != newWindowHandle)
	//{
	//	//m_RenderWindow-create(newWindowHandle);
	//}
}
