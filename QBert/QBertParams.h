#pragma once
#include <cstdint>


namespace QBert
{
	static constexpr float GlobalScaleX{ 2.5f };
	static constexpr float GlobalScaleY{ 2.5f };

	static constexpr float LevelPixelX{ 32.0f };
	static constexpr float LevelPixelY{ 32.0f };

	struct Sound
	{
		static constexpr const char* Jump{ "./Resources/Sounds/Jump.wav" };
		static constexpr const char* Win{ "./Resources/Sounds/Win.wav" };
		static constexpr const char* Lose{ "./Resources/Sounds/Lose.wav" };
	};
	
	struct Level1
	{
		inline static const sf::IntRect RectPlatFormStart{0,224,32,32};
		inline static const sf::IntRect RectPlatFormEnd{0,192,32,32};
	};

	enum class Layer
	{
		Map,
		Player
	};
}
