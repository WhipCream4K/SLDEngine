#include "Player.h"

#include <SLDFramework.h>

#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "Rendering/RenderParams.h"
#include "Components/InputComponent.h"

#include "QBertParams.h"

//#include <SFML/Graphics.hpp>

Player::Player(SLD::WorldEntity& world)
{
	m_World = &world;

	m_GameObject = world.CreateGameObject();

	m_TransformComponent = m_GameObject->GetTransform();
	m_TransformComponent.lock()->GetPtr()->SetScale(2.5f, 2.5f, 1.0f);
	const auto transform{ m_TransformComponent.lock() };

	// Setup Render Component
	const size_t renderData{ sizeof(sf::Sprite) };
	const uint32_t renderElemCnt{ 1 };
	m_RenderingComponent = m_GameObject->CreateRenderingComponent(renderData, renderElemCnt);

	// Sprite Creation
	m_CharacterSprite = m_RenderingComponent->GetPtr()->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));

	transform->GetPtr()->Translate(-20.0f, 0.0f, float(QBert::Layer::Player));

	m_InputComponent = m_GameObject->CreateComponent<SLD::InputComponent>();

	auto inputTest{ m_GameObject->GetComponent<SLD::InputComponent>() };
}

void Player::SetSpriteTexture(const sf::Texture& texture) const
{
	m_CharacterSprite->GetPtr()->setTexture(texture, true);
	//auto ref{ *m_CharacterSprite };
	//ref->setTexture(texture, true);

	// Set to correct sprite rect
	const sf::IntRect spriteRect{
		0,0,16,16
	};
	m_CharacterSprite->GetPtr()->setTextureRect(spriteRect);
	m_CharacterSprite->GetPtr()->setOrigin(16.0f * 0.5f, 16.0f * 0.5f);
	//ref->setTextureRect(spriteRect);
	//ref->setOrigin(16.0f * 0.5f, 16.0f * 0.5f);
}

void Player::SetUpPlayerInput()
{
	if (const auto inputComponent{ m_InputComponent.lock() }; inputComponent)
	{
		inputComponent->GetPtr()->BindAxis("Horizontal", &Player::MoveHorizontal, this);
		inputComponent->GetPtr()->BindAction("MoveDiagonal", SLD::InputEvent::IE_Released, &Player::MoveDiagonal, this);
	}
}

void Player::MoveDiagonal()
{
	m_IsMoving = true;
}

void Player::MoveHorizontal(float value)
{
	if (const auto transform{ m_TransformComponent.lock() })
	{
		const rtm::float3f& pos{ transform->GetPtr()->GetWorldPos() };
		const float speed{ 100.0f };
		const float horizon{ pos.x + (speed * value * m_World->GetDeltaTime()) };
		transform->GetPtr()->Translate(horizon, pos.y, pos.z);
	}
}

void Player::Update(float deltaTime)
{
	deltaTime;
}


void Player::LerpTo(const rtm::float3f& to)
{
	to;
}

std::shared_ptr<SLD::GameObject> Player::GetGameObject()
{
	return m_GameObject;
}
