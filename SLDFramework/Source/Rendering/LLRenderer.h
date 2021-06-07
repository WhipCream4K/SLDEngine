#ifndef SLDFRAMEWORK_LL_RENDERER_H
#define SLDFRAMEWORK_LL_RENDERER_H

#include "DX12/DX12Renderer.h"
#include "SFML/SFML2DRenderer.h"

#include <variant>

namespace SLD
{
	using LLRenderer = std::variant<
#ifdef HAS_DX12
		DX12Renderer,
#endif
//#ifdef HAS_SFML
		SFML2DRenderer
//#endif
	>;

}

#endif
