#ifndef SLDFRAMEWORK_SFMLPREREQUISITE_H
#define SLDFRAMEWORK_SFMLPREREQUISITE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define HAS_SFML

#ifdef HAS_SFML

inline sf::Color Float4ToSfColor(const float (&color)[4])
{
	sf::Color out{};

	out.r = uint8_t(color[0] * 255.0f);
	out.g = uint8_t(color[1] * 255.0f);
	out.b = uint8_t(color[2] * 255.0f);
	out.a = uint8_t(color[3] * 255.0f);
	
	return out;
}

#endif

#endif