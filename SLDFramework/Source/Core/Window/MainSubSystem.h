#ifndef SLDFRAMEWORK_LL_VIEWPORT_H
#define SLDFRAMEWORK_LL_VIEWPORT_H

#include "Windows/WindowsWindow.h"
#include "SFML/SFMLWindow.h"

#include <variant>

namespace SLD
{
	using LLWindow = std::variant<
#if defined(WIN32) && defined(HAS_DX12)
	WindowsWindow,
#endif
#ifdef HAS_SFML
	SFMLWindow
#endif
	>;
}

#endif
