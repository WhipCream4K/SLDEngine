#include "Player.h"

#include <SLDFramework.h>

#include "GameObject/GameObject.h"
#include "Rendering/RenderParams.h"
#include "Components/InputComponent.h"

#include "QBertParams.h"

//#include <SFML/Graphics.hpp>

Player::Player(SLD::WorldEntity& world)
{
	m_World = &world;
	
	m_GameObject = world.CreateGameObject();

	m_TransformComponent = m_GameObject->GetTransform();
	m_TransformComponent.lock()->GetPtr()->SetScale(2.5f, 2.5f,1.0f);
	const auto transform{ m_TransformComponent.lock() };
	
	// Setup Render Component
	size_t renderData{ sizeof(void*) + sizeof(sf::Sprite) };
	m_RenderingComponent = m_GameObject->CreateRenderingComponent(renderData, 2);
	
	// Push transform to Renderer
	m_RenderingComponent->PushElement(SLD::RenderIdentifier(SFMLRenderElement::WorldMatrix),transform.get());

	// Sprite Creation
	m_CharacterSprite = m_RenderingComponent->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));

	transform->GetPtr()->Translate(0.0f, 0.0f, float(QBert::Layer::Player));

	m_InputComponent = m_GameObject->CreateComponent<SLD::InputComponent>();

	auto inputTest{ m_GameObject->GetComponent<SLD::InputComponent>() };

	//m_InputComponent.lock()->GetPtr()->BindAction("Horizontal", SLD::InputEvent::IE_Released, &Player::SetSpriteTexture, this);
}

void Player::SetSpriteTexture(const sf::Texture& texture) const
{
	m_CharacterSprite->setTexture(texture,true);
	
	// Set to correct sprite rect
	const sf::IntRect spriteRect{
		0,0,16,16
	};
	m_CharacterSprite->setTextureRect(spriteRect);
	m_CharacterSprite->setOrigin(16.0f * 0.5f, 16.0f * 0.5f);
}

void Player::SetUpPlayerInput()
{
	if(const auto inputComponent{ m_InputComponent.lock() }; inputComponent)
	{
		inputComponent->GetPtr()->BindAxis("Horizontal", &Player::MoveHorizontal, this);
	}
}

void Player::MoveDiagonal()
{
	
}

void Player::MoveHorizontal(float value)
{
	if (const auto transform{ m_TransformComponent.lock() })
	{
		const rtm::float3f& pos{ transform->GetPtr()->GetWorldPos() };
		const float speed{ 1000.0f };
		const float horizon{ pos.x + (speed * value * m_World->GetDeltaTime()) };
		//m_Horizontal += speed * value * m_World->GetDeltaTime();
		//pos.x += speed * value * m_World->GetDeltaTime();
		transform->GetPtr()->Translate(horizon, pos.y, pos.z);
	}
}

std::shared_ptr<SLD::GameObject> Player::GetGameObject()
{
	return m_GameObject;
}
