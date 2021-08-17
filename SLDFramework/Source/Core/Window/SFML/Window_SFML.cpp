#include "../Window.h"
#include "../../../Miscellaneous/SFMLPrerequisite.h"

void SLD::Window::InternInitialize(uint32_t width, uint32_t height, const std::string& windowTitleName)
{
	m_WindowHandle = std::make_shared<sf::RenderWindow>(sf::VideoMode{ width,height }, windowTitleName);
}

const SLD::Window::WindowMessages& SLD::Window::PollWindowMessages(bool& shouldEarlyExit)
{
	m_WndMessages.totalMessages = 0;

	uint8_t messageCnt{};

	if (m_WindowHandle->isOpen())
	{
		sf::Event sfmlEvent{};
		while (m_WindowHandle->pollEvent(sfmlEvent))
		{
			if (messageCnt > MinimumEventCnt)
				break;

			InputParams::MessageBus2 messageData{};

			switch (sfmlEvent.type)
			{

			case sf::Event::Closed:
			{
				shouldEarlyExit = true;
				return m_WndMessages;
			}
			case sf::Event::GainedFocus:
			{
				OnGainFocus();
				break;
			}
			case sf::Event::LostFocus:
			{
				OnLostFocus();
				break;
			}
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			{
				messageData.data = sfmlEvent.key;
				messageData.size = sizeof(sf::Event::KeyEvent);

				break;
			}
			case sf::Event::Resized:
			{
				messageData.data = sfmlEvent.size;
				messageData.size = sizeof(sf::Event::SizeEvent);

				Resize(sfmlEvent.size.width, sfmlEvent.size.height);
				OnResize(uint32_t(sfmlEvent.size.width), uint32_t(sfmlEvent.size.height));

				break;
			}
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			{

				messageData.data = sfmlEvent.mouseButton;
				messageData.size = sizeof(sf::Event::MouseButtonEvent);

				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				messageData.data = sfmlEvent.mouseWheelScroll;
				messageData.size = sizeof(sf::Event::MouseWheelScrollEvent);

				break;
			}
			case sf::Event::MouseMoved:
			{
				messageData.data = sfmlEvent.mouseMove;
				messageData.size = sizeof(sf::Event::MouseMoveEvent);

				break;
			}

			default: break;
			}

			messageData.eventId = uint8_t(sfmlEvent.type);

			m_WndMessages.wndMessages[messageCnt++] = messageData;
		}

		m_WndMessages.totalMessages = messageCnt;
	}

	return m_WndMessages;
}



void SLD::Window::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	m_WindowHandle->setSize(sf::Vector2u{ width,height });
}


void SLD::Window::SetVSync(bool value)
{
	m_ShouldVSync = value;
	m_WindowHandle->setVerticalSyncEnabled(value);
}


void SLD::Window::Present()
{
	m_WindowHandle->display();
}


void SLD::Window::ClearBackBuffer()
{
	m_WindowHandle->clear(Float4ToSfColor(m_ClearColor));
}

void SLD::Window::InternDestroy() noexcept
{
	m_WindowHandle->close();
}

