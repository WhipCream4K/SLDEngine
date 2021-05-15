#pragma once
#include <cstdint>


namespace QBert
{
	static constexpr float GlobalScaleX{ 2.5f };
	static constexpr float GlobalScaleY{ 2.5f };

	struct Level1
	{
		inline static const sf::IntRect RectPlatFormStart{0,224,32,32};
		inline static const sf::IntRect RectPlatFormEnd{0,192,32,32};
	};
	
}
