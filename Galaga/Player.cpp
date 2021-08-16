#include "Player.h"

#include <Components/TransformComponent.h>
#include <Physics/Components/Box2DComponent.h>
#include <box2d/box2d.h>
#include <Helpers/utils.h>
#include "MyComponents.h"
#include "GalagaScene.h"
#include "ResourceManager.h"
#include "ShootableComponent.h"
#include "CollisionGroup.h"
#include "ParticleManager.h"

Player::Player(size_t playerIndex)
	: m_PlayerIndex(playerIndex)
{
}

void Player::OnCreate(const SharedPtr<SLD::GameObject>& gameObject)
{
	using namespace SLD;

	m_Instance = gameObject;

	TransformComponent* transform{ gameObject->GetComponent<TransformComponent>() };
	const auto& worldPos{ transform->GetWorldPos() };

	auto& world{ gameObject->GetWorld() };

	sf::Texture* mainTexture{ Instance<ResourceManager>()->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };

	if (mainTexture)
	{
		m_Instance->AddComponent<SpriteRenderComponent>({
			*mainTexture,PlayerSpriteRect,GalagaScene::GlobalScale,{0.5f,0.5f} });
	}

	const float objectWidth{ PlayerSpriteRect.z * GalagaScene::GlobalScale.x };
	const float objectHeight{ PlayerSpriteRect.a * GalagaScene::GlobalScale.y };

	b2BodyDef bodyDef{ CreateBasicBody(GetPlayerId(),b2_kinematicBody) };
	bodyDef.position = { worldPos.x,worldPos.y };

	//SharedPtr<b2PolygonShape> box{std::make_shared<b2PolygonShape>()};
	//box->SetAsBox(objectWidth, objectHeight);

	b2PolygonShape box{};
	box.SetAsBox(objectWidth, objectHeight);
	
	b2FixtureDef fixtureDef{};
	fixtureDef.filter.categoryBits = CollisionGroup::mPlayer;
	//fixtureDef.shape = box.get();
	fixtureDef.shape = &box;

	//m_Instance->AddComponent<Box2DComponent>({ world,bodyDef,fixtureDef,box,objectWidth,objectHeight });
	m_Instance->AddComponent<Box2DComponent>({ world,bodyDef,fixtureDef,objectWidth,objectHeight });

	const float reloadTime{ 0.3f };
	m_Instance->AddComponent<InputListener>({ m_PlayerIndex });
	m_Instance->AddComponent<ShootableComponent>({ CollisionGroup::mPlayer,reloadTime });
	m_Instance->AddComponent<SpeedComponent>({ 1000.0f });
	m_Instance->AddComponent<PlayerTag>();
}

SLD::GameObjectId Player::GetPlayerId() const
{
	return m_Instance->GetId();
}

void Player::OnDestroy(SLD::GameObjectId)
{

}