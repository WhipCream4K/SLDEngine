
#ifndef SLDFRAMEWORK_SFMLSUBSYSTEM_H
#define SLDFRAMEWORK_SFMLSUBSYSTEM_H

#include "../../Core/Base.h"
#include "../../Miscellaneous/SFMLPrerequisite.h"
#include "../../Inputs/InputParams.h"

#include <any>

#ifdef HAS_SFML

class SFMLWindow
{
public:

	SFMLWindow(const std::any& windowHandle,
		[[maybe_unused]] uint32_t width, 
		[[maybe_unused]] uint32_t height);

	SFMLWindow(const SFMLWindow&) = delete;
	SFMLWindow& operator=(const SFMLWindow&) = delete;
	SFMLWindow(SFMLWindow&& other) noexcept;
	SFMLWindow& operator=(SFMLWindow&& other) noexcept;

	sf::RenderWindow& GetSubRef();
	RefPtr<sf::RenderWindow> GetSubRefPtr();

	SLD::InputParams::ReadOut ReadUserInputs();
	void Resize(uint32_t width,uint32_t height);
	void ClearColor(const float(&clearColor)[4]);
	void Present(bool shouldVSync);

private:

	sf::RenderWindow m_MainWindow;
};

#endif


#endif
