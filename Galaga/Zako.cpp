#include "Zako.h"
#include "MyComponents.h"
#include "GalagaScene.h"
#include "ResourceManager.h"
#include "CollisionGroup.h"
#include "EnemyPath.h"
#include "ShootableComponent.h"
#include "OnObjectHit.h"
#include <Physics/Components/Box2DComponent.h>
#include <Components/TransformComponent.h>


Zako::Zako(SpawnDirection direction)
	: m_Instance()
	, m_SpawnDirection(direction)
	, m_Col()
	, m_Row()
{
}

Zako::Zako(SpawnDirection dir, int startRow, int startCol)
	: m_SpawnDirection(dir)
	, m_Col(startCol)
	, m_Row(startRow)
{
}

void Zako::OnCreate(const SharedPtr<SLD::GameObject>& gameObject)
{
	using namespace SLD;

	m_Instance = gameObject;

	const auto& worldPos{ gameObject->GetComponent<TransformComponent>()->GetWorldPos() };

	b2BodyDef bodyDef{};
	bodyDef.userData.pointer = gameObject->GetId();
	bodyDef.type = b2_kinematicBody;
	bodyDef.position = { worldPos.x,worldPos.y };

	const float objectWidth{ float(ZakoRect.z) * 1.25f };
	const float objectHeight{ float(ZakoRect.a) * 1.25f};

	//SharedPtr<b2PolygonShape> box{ std::make_shared<b2PolygonShape>() };
	//box->SetAsBox(objectWidth, objectHeight);

	b2PolygonShape box{};
	box.SetAsBox(objectWidth, objectHeight);

	b2FixtureDef fixtureDef{};
	//fixtureDef.shape = box.get();
	fixtureDef.shape = &box;
	fixtureDef.filter.categoryBits = CollisionGroup::Enemy;
	fixtureDef.filter.maskBits -= CollisionGroup::Enemy;

	////gameObject->AddComponent<Box2DComponent>({ gameObject->GetWorld(),bodyDef,fixtureDef,box,objectWidth,objectHeight });
	gameObject->AddComponent<Box2DComponent>({ gameObject->GetWorld(),bodyDef,fixtureDef,objectWidth,objectHeight });

	sf::Texture* mainTexture{ Instance<ResourceManager>()->Get<sf::Texture>("Zako")};

	if (mainTexture)
		gameObject->AddComponent<SpriteRenderComponent>({ *mainTexture,{0,0,16,16},GalagaScene::GlobalScale });

	gameObject->AddComponent<EnemyTag>({ EnemyType::Zako });

	int startHealth{ 1 };
	gameObject->AddComponent<HealthComponent>({ startHealth });

	const float speed{ 210.0f };
	gameObject->AddComponent<SpeedComponent>({ speed });

	gameObject->AddComponent<FlyInComponent>({ Instance<EnemyPath>()->GetPath(m_SpawnDirection) });

	gameObject->AddComponent<FormationComponent>({ m_Row,m_Col });

	gameObject->AddComponent<ScoreComponent>({ 50 });

	gameObject->AddComponent<ShootableComponent>({ CollisionGroup::Enemy,1.0f });
	gameObject->AddComponent<DiveComponent>();
	gameObject->AddComponent<OnHitCommand>({std::make_shared<EnemyHit>()});
}

const SharedPtr<SLD::GameObject>& Zako::GetGameObjectInstance() const
{
	return m_Instance;
}
