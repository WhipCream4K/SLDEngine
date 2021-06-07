#pragma once
#include <cstdint>


namespace QBert
{
	static constexpr float GlobalScaleX{ 2.5f };
	static constexpr float GlobalScaleY{ 2.5f };

	static constexpr float LevelPixelX{ 32.0f };
	static constexpr float LevelPixelY{ 32.0f };

	static constexpr uint32_t PlayerMaxLives{ 3u };
	
	struct Sound
	{
		static constexpr const char* Jump{ "./Resources/Sounds/Jump.wav" };
		static constexpr const char* Win{ "./Resources/Sounds/Win.wav" };
		static constexpr const char* Lose{ "./Resources/Sounds/Lose.wav" };
		static constexpr const char* Disc{ "./Resources/Sounds/Disc.wav" };
	};

	struct HUDElements
	{
	};

	struct ScoreType
	{
		inline static const int ChangeColor{ 25 };
		inline static const int DiscBonus{ 50 };
	};
	
	struct Level1
	{
		inline static const sf::IntRect SpriteStart{0,224,32,32};
		inline static const sf::IntRect SpriteEnd{0,192,32,32};
		inline static const sf::IntRect SpriteSpare{ 0,160,32,32 };
		inline static const sf::IntRect PlatfromChangeTo{ 64,192,16,16 };
		inline static const sf::IntRect FlyingDisc{0,352,16,16};
	};

	struct Level2
	{
		inline static const sf::IntRect SpriteStart{ 80,160,32,32 };
		inline static const sf::IntRect SpriteSpare{ 80,192,32,32 };
		inline static const sf::IntRect SpriteEnd{ 80,224,32,32 };
		inline static const sf::IntRect PlatfromChangeTo{ 144,224,16,16 };
		inline static const sf::IntRect FlyingDisc{80,352,16,16};
	};

	struct Level3
	{
		inline static const sf::IntRect SpriteStart{ 160,192,32,32 };
		inline static const sf::IntRect SpriteSpare{ 160,224,32,32 };
		inline static const sf::IntRect SpriteEnd{ 160,160,32,32 };
		inline static const sf::IntRect PlatfromChangeTo{ 224,160,16,16 };
		inline static const sf::IntRect FlyingDisc{160,352,16,16};
	};

	enum class Layer
	{
		BackGround,
		Map,
		Player,
		HUD
	};
}
