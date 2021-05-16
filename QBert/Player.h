#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Core/ObservePtr.h"

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

	void SetSpriteTexture(const sf::Texture& texture);

private:

	std::shared_ptr<sf::Sprite> m_CharacterSprite;
	std::shared_ptr<sf::CircleShape> m_TestCircle;
	std::shared_ptr<SLD::RenderingComponent> m_RenderingComponent;
	SLD::ObservePtr<SLD::TransformComponent> m_TransformComponent;
	std::shared_ptr<SLD::GameObject> m_GameObject;
};

