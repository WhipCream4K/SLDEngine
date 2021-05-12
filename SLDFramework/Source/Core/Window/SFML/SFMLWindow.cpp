#include "SFMLWindow.h"

#ifdef HAS_SFML

SFMLWindow::SFMLWindow(const std::any& windowHandle, uint32_t width, uint32_t height)
	: m_MainWindow()
{
	sf::WindowHandle handle = std::any_cast<sf::WindowHandle>(windowHandle);
	m_MainWindow.create(handle);
	width;
	height;
}

SFMLWindow::SFMLWindow(SFMLWindow&& other) noexcept
	: m_MainWindow()
{
	sf::WindowHandle handle{ other.m_MainWindow.getSystemHandle() };
	other.m_MainWindow.close();
	m_MainWindow.create(handle);
}

SFMLWindow& SFMLWindow::operator=(SFMLWindow&& other) noexcept
{
	if(this != &other)
	{
		sf::WindowHandle handle{ other.m_MainWindow.getSystemHandle() };
		other.m_MainWindow.close();
		m_MainWindow.create(handle);
	}

	return *this;
}

sf::RenderWindow& SFMLWindow::GetSubRef()
{
	return m_MainWindow;
}


RefPtr<sf::RenderWindow> SFMLWindow::GetSubRefPtr() 
{
	return RefPtr<sf::RenderWindow>{&m_MainWindow, [](sf::RenderWindow*)
	{
	}};
}

SLD::InputParams::ReadOut SFMLWindow::ReadUserInputs()
{
	SLD::InputParams::ReadOut out{};

	sf::Event ev{};
	uint8_t cnt{};


	while (m_MainWindow.pollEvent(ev))
	{
		if (cnt == SLD::MinimumEventCnt)
			break;

		SLD::InputParams::EventData evData{};

		switch (ev.type)
		{
		case sf::Event::Closed:
		{
			out.isExit = true;
			//m_MainWindow.close();
			return out;
		}
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
		{
			evData.keyboard.alt = ev.key.alt;
			evData.keyboard.ctrl = ev.key.control;
			evData.keyboard.shift = ev.key.shift;
			evData.keyboard.system = ev.key.system;

			evData.keyboard.key = uint8_t(ev.key.code);

			break;
		}
		case sf::Event::Resized:
		{
			evData.windowSize.width = uint32_t(ev.size.width);
			evData.windowSize.height = uint32_t(ev.size.height);

			break;
		}
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		{
			auto& outMouseKey{ evData.mouse.key };

			switch (ev.mouseButton.button)
			{
			case sf::Mouse::Left:		outMouseKey = SLD::InputParams::MouseKey::MK_Left;	break;
			case sf::Mouse::Right:		outMouseKey = SLD::InputParams::MouseKey::MK_Right;	break;
			case sf::Mouse::Middle:		outMouseKey = SLD::InputParams::MouseKey::MK_Middle;	break;
			case sf::Mouse::XButton1:	outMouseKey = SLD::InputParams::MouseKey::MK_Thumb1;	break;
			case sf::Mouse::XButton2:	outMouseKey = SLD::InputParams::MouseKey::MK_Thumb2;	break;
			default:														break;
			}

			evData.mouse.x = float(ev.mouseWheelScroll.x);
			evData.mouse.y = float(ev.mouseWheelScroll.y);

			break;
		}
		case sf::Event::MouseWheelScrolled:
		{
			evData.mouse.verticalScroll = ev.mouseWheelScroll.delta;
			evData.mouse.x = float(ev.mouseWheelScroll.x);
			evData.mouse.y = float(ev.mouseWheelScroll.y);
			break;
		}
		case sf::Event::MouseMoved:
		{
			evData.mouse.x = float(ev.mouseMove.x);
			evData.mouse.y = float(ev.mouseMove.y);
			break;
		}
		default: break;
		}

		out.buses[cnt++].data = evData;
		out.buses[cnt++].eventId = uint8_t(ev.type);

	}

	out.eventCnt = cnt;


	return out;
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

	//if(m_MainWindow.isOpen())
	//{
	//	bool some{};
	//	sf::Event ev{};
	//	while(m_MainWindow.pollEvent(ev))
	//	{
	//		switch (ev.type)
	//		{
	//		case sf::Event::Closed:
	//			some = false;
	//			break;
	//		}
	//	}
	//}
	m_MainWindow.setVerticalSyncEnabled(shouldVSync);
	m_MainWindow.display();
}

#endif
