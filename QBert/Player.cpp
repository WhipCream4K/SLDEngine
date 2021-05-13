#include "Player.h"

#include <SLDFramework.h>

#include "GameObject/GameObject.h"
#include "Rendering/RenderParams.h"

Player::Player(SLD::WorldEntity& world)
{
	m_GameObject = world.CreateGameObject();

	m_TransformComponent = m_GameObject->GetTransform();
	m_TransformComponent.lock()->SetScale(2.5f, 2.5f,1.0f);
	
	// Setup Render Component
	size_t renderData{ sizeof(void*) + sizeof(sf::CircleShape) };
	m_RenderingComponent = m_GameObject->CreateRenderingComponent(renderData, 2);
	
	// Push transform to Renderer
	m_RenderingComponent->PushElement(SLD::RenderIdentifier(SFMLRenderElement::WorldMatrix),&m_TransformComponent.lock()->GetWorldFinishMatrix());

	// Sprite Creation
	//m_CharacterSprite = m_RenderingComponent->AllocAndConstructData<sf::Sprite>(SLD::RenderIdentifier(SFMLRenderElement::RenderSprite));

	// TEST
	m_TestCircle = m_RenderingComponent->AllocAndConstructData<sf::CircleShape>(
		SLD::RenderIdentifier(SFMLRenderElement::RenderShapes),
		50.0f
		);

	sf::FloatRect bounding{ m_TestCircle->getLocalBounds() };
	m_TestCircle->setOrigin(bounding.width / 2.0f, bounding.height / 2.0f);
	
}
