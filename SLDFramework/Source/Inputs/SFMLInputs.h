
#ifndef SLDFRAMEWORK_SFMLHANDLER_H
#define SLDFRAMEWORK_SFMLHANDLER_H

#include "InputParams.h"
#include "../Core/Base.h"

#include "SFML/Graphics.hpp"
//#include <SFML/Window.hpp>
//#include <any>

class SFMLWindow;

// encapsulate the process of sfml inputs in here
class SFMLInputs final
{
public:

	SFMLInputs(const std::any& window);
	//SFMLInputs(const RefPtr<sf::Window>& window);
	//SFMLInputs(const RefPtr<SFMLWindow>& window);
	SFMLInputs(SFMLWindow* ref);
	SFMLInputs(SFMLInputs&& other) noexcept;
	SLD::InputParams::ReadOut ReadInputs();

private:

	//RefPtr<sf::Window> m_Window;
	sf::Window m_InputWindow;
	//RefPtr<SFMLWindow> m_RefWindow;
	//RefPtr<sf::RenderWindow> m_TestRef2;
	//SFMLWindow* m_TestRef;
};


#endif