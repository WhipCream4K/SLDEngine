#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace SLD
{
	class WorldEntity;
	class GameObject;
	class RenderingComponent;
	class TransformComponent;
}

class Player
{
public:

	Player(SLD::WorldEntity& world);
	
private:

	std::shared_ptr<sf::Sprite> m_CharacterSprite;
	std::shared_ptr<sf::CircleShape> m_TestCircle;
	std::shared_ptr<SLD::RenderingComponent> m_RenderingComponent;
	std::weak_ptr<SLD::TransformComponent> m_TransformComponent;
	std::shared_ptr<SLD::GameObject> m_GameObject;
};

