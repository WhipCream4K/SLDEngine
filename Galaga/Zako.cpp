#include "Zako.h"
#include "MyComponents.h"
#include "GalagaScene.h"
#include "ResourceManager.h"
#include "CollisionGroup.h"
#include "EnemyPath.h"
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

	const float objectWidth{ ZakoRect.z * GalagaScene::GlobalScale.x };
	const float objectHeight{ ZakoRect.a * GalagaScene::GlobalScale.y };

	//SharedPtr<b2PolygonShape> box{ std::make_shared<b2PolygonShape>() };
	//box->SetAsBox(objectWidth, objectHeight);

	b2PolygonShape box{};
	box.SetAsBox(objectWidth, objectHeight);

	b2FixtureDef fixtureDef{};
	//fixtureDef.shape = box.get();
	fixtureDef.shape = &box;
	fixtureDef.filter.categoryBits = CollisionGroup::Enemy;
	fixtureDef.filter.maskBits -= CollisionGroup::Enemy;

	//gameObject->AddComponent<Box2DComponent>({ gameObject->GetWorld(),bodyDef,fixtureDef,box,objectWidth,objectHeight });
	gameObject->AddComponent<Box2DComponent>({ gameObject->GetWorld(),bodyDef,fixtureDef,objectWidth,objectHeight });

	sf::Texture* mainTexture{ Instance<ResourceManager>()->Get<sf::Texture>(GalagaScene::MainSpriteSheet) };

	if (mainTexture)
		gameObject->AddComponent<SpriteRenderComponent>({ *mainTexture,ZakoRect,GalagaScene::GlobalScale });

	gameObject->AddComponent<EnemyTag>({ EnemyType::Zako });

	int startHealth{ 1 };
	gameObject->AddComponent<HealthComponent>({ startHealth });

	const float speed{ 1000.0f };
	gameObject->AddComponent<SpeedComponent>({ speed });

	gameObject->AddComponent<FlyInComponent>({ Instance<EnemyPath>()->GetPath(m_SpawnDirection) });

	gameObject->AddComponent<FormationComponent>({ m_Row,m_Col });
}

const SharedPtr<SLD::GameObject>& Zako::GetGameObjectInstance() const
{
	return m_Instance;
}
