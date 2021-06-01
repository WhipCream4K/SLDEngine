#include "SFMLWindow.h"
#include "../../Miscellaneous/SFMLPrerequisite.h"

class SFMLWindow::ImplSFMLWindow
{
public:

	ImplSFMLWindow(sf::WindowHandle winHandle);
	~ImplSFMLWindow();

	void Resize(uint32_t width, uint32_t height);
	void ClearColor(const float(&clearColor)[4]);
	void Present(bool shouldVSync);
	SLD::InputParams::ReadOut ReadUserInputs();
	sf::RenderWindow& GetWindow();

private:
	
	sf::RenderWindow m_MainWindow{};
};

SFMLWindow::ImplSFMLWindow::ImplSFMLWindow(sf::WindowHandle winHandle)
	: m_MainWindow(winHandle)
{	
}

SFMLWindow::ImplSFMLWindow::~ImplSFMLWindow()
{
	m_MainWindow.close();
}

void SFMLWindow::ImplSFMLWindow::Resize(uint32_t width, uint32_t height)
{
	m_MainWindow.setSize(sf::Vector2u{ width,height });
}

void SFMLWindow::ImplSFMLWindow::ClearColor(const float(&clearColor)[4])
{
	m_MainWindow.clear(Float4ToSfColor(clearColor));
}

void SFMLWindow::ImplSFMLWindow::Present(bool shouldVSync)
{
	m_MainWindow.setVerticalSyncEnabled(shouldVSync);
	m_MainWindow.display();
}

SLD::InputParams::ReadOut SFMLWindow::ImplSFMLWindow::ReadUserInputs()
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

		out.buses[cnt].data = evData;
		out.buses[cnt].eventId = uint8_t(ev.type);
		cnt++;
	}

	out.eventCnt = cnt;


	return out;
}

sf::RenderWindow& SFMLWindow::ImplSFMLWindow::GetWindow()
{
	return m_MainWindow;
}

SFMLWindow::SFMLWindow(
	const std::any& windowHandle,
	[[maybe_unused]] uint32_t width,
	[[maybe_unused]] uint32_t height)
	: m_pMainWindow(std::make_unique<ImplSFMLWindow>(std::any_cast<sf::WindowHandle>(windowHandle)))
{
}

SFMLWindow::~SFMLWindow() = default;

SFMLWindow::SFMLWindow(SFMLWindow&& other) noexcept
	: m_pMainWindow(std::move(other.m_pMainWindow))
{
}

SFMLWindow& SFMLWindow::operator=(SFMLWindow&& other) noexcept
{
	if(this != &other)
	{
		m_pMainWindow = std::move(other.m_pMainWindow);
	}

	return *this;
}

SLD::InputParams::ReadOut SFMLWindow::ReadUserInputs()
{
	return m_pMainWindow->ReadUserInputs();
}

sf::RenderWindow& SFMLWindow::GetSFMLWindow()
{
	return m_pMainWindow->GetWindow();
}

void SFMLWindow::Resize(uint32_t width, uint32_t height)
{
	m_pMainWindow->Resize(std::move(width), std::move(height));
}

void SFMLWindow::ClearColor(const float(&clearColor)[4])
{
	m_pMainWindow->ClearColor(clearColor);
}

void SFMLWindow::Present(bool shouldVSync)
{
	m_pMainWindow->Present(shouldVSync);
}
