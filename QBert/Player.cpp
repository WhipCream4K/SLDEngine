#include "Player.h"

#include <SLDFramework.h>

#include "GameObject/GameObject.h"
#include "Rendering/RenderParams.h"

Player::Player(SLD::WorldEntity& world)
{
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

	transform->GetPtr()->Translate(-500.0f, 200.0f, 0.0f);
}

void Player::SetSpriteTexture(const sf::Texture& texture)
{
	m_CharacterSprite->setTexture(texture,true);
	
	// Set to correct sprite rect
	const sf::IntRect spriteRect{
		0,0,16,16
	};
	m_CharacterSprite->setTextureRect(spriteRect);
	m_CharacterSprite->setOrigin(16.0f, 16.0f);
}
