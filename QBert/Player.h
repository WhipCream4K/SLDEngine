#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Core/ObservePtr.h"

namespace SLD
{
	class InputComponent;
	class WorldEntity;
	class GameObject;
	class RenderingComponent;
	class TransformComponent;
}

class Player
{
public:

	Player(SLD::WorldEntity& world);

	void SetSpriteTexture(const sf::Texture& texture) const;
	void SetUpPlayerInput();

	void MoveHorizontal(float value);
	

private:

	std::shared_ptr<sf::Sprite> m_CharacterSprite;
	std::shared_ptr<sf::CircleShape> m_TestCircle;
	std::shared_ptr<SLD::RenderingComponent> m_RenderingComponent;
	std::weak_ptr<SLD::ObservePtr<SLD::TransformComponent>> m_TransformComponent;
	std::weak_ptr<SLD::ObservePtr<SLD::InputComponent>> m_InputComponent;
	std::shared_ptr<SLD::GameObject> m_GameObject;
	bool m_IsMoving{};
};

