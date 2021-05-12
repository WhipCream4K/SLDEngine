#include "SFMLInputs.h"
#include "InputManager.h"

#include <SFML/Graphics.hpp>

#include "../Core/Window/SFML/SFMLWindow.h"

//SLD::SFMLInputs::SFMLInputs(const std::any& window)
//	:(/*std::any_cast<sf::WindowHandle>(window)*/)
//{
//	//sf::WindowHandle handle{ std::any_cast<sf::WindowHandle>(window) };
//	//m_Window.create(handle);
//	window;
//}

//SFMLInputs::SFMLInputs(const RefPtr<SFMLWindow>& window)
//	: m_InputWindow(window)
//{
//}

SFMLInputs::SFMLInputs(const std::any& window)
	: m_InputWindow()
{
	window;
	//sf::WindowHandle handle{ std::any_cast<sf::WindowHandle>(window) };
	//m_InputWindow.create(handle);
}

//SFMLInputs::SFMLInputs(SFMLWindow* ref)
//	: m_TestRef(ref)
//{
//}

SFMLInputs::SFMLInputs(SFMLInputs&& other) noexcept
	: m_InputWindow()
{
	//sf::WindowHandle handle{ other.m_InputWindow.getSystemHandle() };
	//other.m_InputWindow.close();
	//m_InputWindow.create(handle);
	other;
}

SLD::ReadOut SFMLInputs::ReadInputs()
{
	SLD::ReadOut out{};

	sf::Event ev{};
	uint8_t cnt{};
	
	//auto& sfmlWindow{ m_TestRef->GetSubRef() };
	
	while (m_InputWindow.pollEvent(ev))
	{
		if (cnt == SLD::MinimumEventCnt)
			break;

		SLD::EventData evData{};

		switch (ev.type)
		{
		case sf::Event::Closed:
		{
			out.isExit = true;
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
			case sf::Mouse::Left:		outMouseKey = SLD::MouseKey::MK_Left;	break;
			case sf::Mouse::Right:		outMouseKey = SLD::MouseKey::MK_Right;	break;
			case sf::Mouse::Middle:		outMouseKey = SLD::MouseKey::MK_Middle;	break;
			case sf::Mouse::XButton1:	outMouseKey = SLD::MouseKey::MK_Thumb1;	break;
			case sf::Mouse::XButton2:	outMouseKey = SLD::MouseKey::MK_Thumb2;	break;
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
