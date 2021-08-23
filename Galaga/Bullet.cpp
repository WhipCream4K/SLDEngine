#include "Bullet.h"
#include "MyComponents.h"
#include "GalagaScene.h"
#include <Physics/Components/Box2DComponent.h>
#include <Components/TransformComponent.h>
#include "CollisionGroup.h"
#include "ResourceManager.h"

void Bullet::OnCreate(const SharedPtr<SLD::GameObject>& gameObject)
{
	using namespace SLD;

	TransformComponent* transform{ gameObject->GetComponent<TransformComponent>() };
	const auto& worldPos{ transform->GetWorldPos() };
	
	b2BodyDef bodyDef{};
	bodyDef.userData.pointer = gameObject->GetId();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { worldPos.x,worldPos.y };

	sf::Texture* mainTexture{ Instance<ResourceManager>()->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };

	if (mainTexture)
		gameObject->AddComponent<SpriteRenderComponent>({ *mainTexture,BulletRect,GalagaScene::GlobalScale });

	const float boxWidth{ 5.0f * GalagaScene::GlobalScale.x };
	const float boxHeight{ 5.0f * GalagaScene::GlobalScale.y };
	
	b2PolygonShape box{};
	box.SetAsBox(boxWidth, boxHeight);
	
	//SharedPtr<b2PolygonShape> box{std::make_shared<b2PolygonShape>()};
	//box->SetAsBox(boxWidth, boxHeight);
	
	b2FixtureDef fixture{};
	//fixture.shape = box.get();
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.categoryBits = CollisionGroup::Projectile;
	fixture.filter.maskBits |= ~CollisionGroup::Projectile;
	
	//gameObject->AddComponent<Box2DComponent>({
	//	gameObject->GetWorld(),
	//	bodyDef,
	//	fixture,
	//	box,
	//	boxWidth,
	//	boxHeight
	//	});
	
	gameObject->AddComponent<Box2DComponent>({
		gameObject->GetWorld(),
		bodyDef,
		fixture,
		boxWidth,
		boxHeight
		});
	
	gameObject->AddComponent<ProjectileComponent>({400.0f,true});
}
