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

	void MoveDiagonal();
	void MoveHorizontal(float value);
	std::shared_ptr<SLD::GameObject> GetGameObject();

private:

	SLD::WorldEntity* m_World;
	std::shared_ptr<sf::Sprite> m_CharacterSprite;
	std::shared_ptr<sf::CircleShape> m_TestCircle;
	std::shared_ptr<SLD::RenderingComponent> m_RenderingComponent;
	std::weak_ptr<SLD::ObservePtr<SLD::TransformComponent>> m_TransformComponent;
	std::weak_ptr<SLD::ObservePtr<SLD::InputComponent>> m_InputComponent;
	std::shared_ptr<SLD::GameObject> m_GameObject;
	float m_Horizontal{};
	bool m_IsMoving{};
};

