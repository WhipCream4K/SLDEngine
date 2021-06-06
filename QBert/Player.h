#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Level.h"
#include "Core/ObservePtr.h"
#include "Math/rtm/vector4f.h"
#include "Core/DynamicMulticastDelegate.h"

class Level;

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

	static const rtm::float3f SpawnPoint;

	using FPlayerFinishedJump = SLD::DynamicMulticastDelegate<void(const Level::Node&)>;

	static FPlayerFinishedJump OnPlayerFinishedJump;
	
	enum class MoveDirection
	{
		None,
		DownRight,
		DownLeft,
		UpRight,
		UpLeft
	};

	Player(SLD::WorldEntity& world);
	Player(SLD::WorldEntity& world, const std::shared_ptr<Level>& gameLevel);

	void SetSpriteTexture(const sf::Texture& texture) const;
	void SetUpPlayerInput();

	void SetPos(const rtm::float3f& pos) const;
	void MoveUpRight();
	void MoveUpLeft();
	void MoveDownRight();
	void MoveDownLeft();
	void MoveHorizontal(float value);
	void Update(float deltaTime);
	void LerpTo(const rtm::float3f& to);
	std::shared_ptr<SLD::GameObject> GetGameObject();
	void SetCurrentNode(uint32_t row, uint32_t col);

private:

	rtm::float3f CalculatePath(MoveDirection dir,Level::Node& outNode);
	
	SLD::WorldEntity* m_World;
	std::weak_ptr<Level> m_GameLevel;
	//std::shared_ptr<SLD::ObservePtr<sf::Sprite>> m_CharacterSprite;
	std::shared_ptr<sf::Sprite> m_CharacterSprite;
	RefPtr<SLD::Tracker> m_CharacterSpriteHandle;
	std::shared_ptr<sf::CircleShape> m_TestCircle;
	//std::weak_ptr<SLD::ObservePtr<SLD::RenderingComponent>> m_RenderingComponent;
	std::weak_ptr<SLD::ObservePtr<SLD::TransformComponent>> m_TransformComponent;
	std::weak_ptr<SLD::ObservePtr<SLD::InputComponent>> m_InputComponent;
	std::shared_ptr<SLD::GameObject> m_GameObject;
	Level::Node m_CurrentNode;
	MoveDirection m_MoveDirection;
	rtm::float3f m_CalculatedLocation;
	float m_MoveSpeed{};
	bool m_IsMoving{};
	bool m_IsDead{};
};

