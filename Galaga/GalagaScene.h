#pragma once

#include "Core/Base.h"
#include <Core/DynamicMulticastDelegate.h>

class PlayArea;

namespace SLD {
	class Core;
}

namespace sf
{
	class Texture;
}

class Player;
class GalagaScene
{
public:
	
	enum class Layer
	{
		Background,
		Particle,
		Enemy,
		Player
	};
	
	GalagaScene();

	void WorldCreation();
	void Run();

	inline static constexpr rtm::float2f GlobalScale{ 3.0f,3.0f };
	
	static constexpr const char* MainSpriteSheet{ "GalagaSprite" };

private:

	SharedPtr<SLD::Core> m_Framework;
	SharedPtr<Player> m_Player;
	SharedPtr<sf::Texture> m_MainSpriteSheet;
	size_t m_GIdTest;
};

 