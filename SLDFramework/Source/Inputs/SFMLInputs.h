
#ifndef SLDFRAMEWORK_SFMLHANDLER_H
#define SLDFRAMEWORK_SFMLHANDLER_H

#include "InputParams.h"
#include "../Core/Base.h"

#include <SFML/Window.hpp>
#include <any>

namespace SLD
{
	// encapsulate the process of sfml inputs in here
	class SFMLInputs final
	{
	public:

		SFMLInputs(const std::any& window);
		SFMLInputs(SFMLInputs&& other) noexcept;
		InputParams::ReadOut ReadInputs();

	private:

		sf::Window m_Window;
	};
}

#endif