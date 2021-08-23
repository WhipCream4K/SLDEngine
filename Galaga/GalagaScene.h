#pragma once

#include "Core/Base.h"
#include <Core/DynamicMulticastDelegate.h>

class PlayArea;

namespace SLD {
	class Core;
}

namespace sf
{
	class Font;
	class Texture;
}

class Player;
class GalagaScene
{
public:
	
	enum class Layer
	{
		Background,
		Enemy,
		Player,
		Particle
	};
	
	GalagaScene();

	void WorldCreation();
	void Run();

	inline static constexpr rtm::float2f GlobalScale{ 3.0f,3.0f };
	
	static constexpr const char* MainSpriteSheet{ "GalagaSprite" };
	static constexpr const char* MainFont{ "MainFont" };
	static constexpr size_t MaxEnemyCount{40};

private:

	SharedPtr<SLD::Core> m_Framework;
	SharedPtr<Player> m_Player;
	SharedPtr<sf::Texture> m_MainSpriteSheet;
	SharedPtr<sf::Font> m_MainFont;
	size_t m_GIdTest;
};

 