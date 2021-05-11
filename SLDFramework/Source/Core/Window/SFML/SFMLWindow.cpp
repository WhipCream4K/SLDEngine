#include "SFMLWindow.h"

#ifdef HAS_SFML

SFMLWindow::SFMLWindow(const std::any& windowHandle, uint32_t width, uint32_t height)
	: m_MainWindow(std::any_cast<sf::WindowHandle>(windowHandle))
{
	width;
	height;
}

SFMLWindow::SFMLWindow(SFMLWindow&& other) noexcept
	: m_MainWindow(other.m_MainWindow.getSystemHandle())
{
	other.m_MainWindow.close();
}

void SFMLWindow::Resize(uint32_t width, uint32_t height)
{
	const auto& view{ m_MainWindow.getView() };
	m_MainWindow.setView({ view.getCenter(),sf::Vector2f{float(width),float(height)} });
}

void SFMLWindow::ClearColor(const float(&clearColor)[4])
{
	m_MainWindow.clear(Float4ToSfColor(clearColor));
}

void SFMLWindow::Present(bool shouldVSync)
{
	m_MainWindow.setVerticalSyncEnabled(shouldVSync);
	m_MainWindow.display();
}

#endif
