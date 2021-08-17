#include "Box2DComponent.h"

#include "../../Core/WorldEntity.h"
#include "../../Components/TransformComponent.h"
#include "PhysicsUserData.h"

SLD::Box2DComponent::Box2DComponent(WorldEntity& world, const b2BodyDef& bodyDef, const b2Shape& shape, float density)
	: m_Dimension{}
{
	//PhysicsUserData data{};

	//b2FixtureDef fixtureDef{};
	//fixtureDef.shape = &shape;
	//fixtureDef.density = density;
	//fixtureDef.userData.pointer = bodyDef.userData.pointer;
	//
	//data.fixtureDef = fixtureDef;
	//data.bodyDef = bodyDef;

	//world.AddPhysiscsBodyDef(data);
	
	const auto& physicsWorld{ world.GetPhysicsWorld() };

	m_PhysicsBody = SharedPtr<b2Body>{ physicsWorld->CreateBody(&bodyDef),[&world](b2Body* p)
	{
		if (p)
		{
			if (const auto& physicsWorld{world.GetPhysicsWorld()};
				physicsWorld)
			{
				physicsWorld->DestroyBody(p);
			}
		}
	} };

	m_PhysicsBody->CreateFixture(&shape, density);
}

SLD::Box2DComponent::Box2DComponent(WorldEntity& world, const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef,
	float width, float height)
	: m_Dimension({width,height})
{

	//PhysicsUserData data{ };
	//

	//data.fixtureDef = fixtureDef;
	//data.fixtureDef.userData.pointer = bodyDef.userData.pointer;
	//data.bodyDef = bodyDef;

	//world.AddPhysiscsBodyDef(data);
	const auto& physicsWorld{ world.GetPhysicsWorld() };

	m_PhysicsBody = SharedPtr<b2Body>{ physicsWorld->CreateBody(&bodyDef),[&world](b2Body* p)
	{
		if (p)
		{
			if (const auto& physicsWorld{world.GetPhysicsWorld()};
				physicsWorld)
			{
				physicsWorld->DestroyBody(p);
			}
		}
	} };

	const auto& [x, y] = m_PhysicsBody->GetPosition();

	m_Position.x = x;
	m_Position.y = y;
	
	b2Fixture* fixture{ m_PhysicsBody->CreateFixture(&fixtureDef) };
	fixture->GetUserData().pointer = bodyDef.userData.pointer;
}

SLD::Box2DComponent::Box2DComponent(WorldEntity& world, const b2BodyDef& bodyDef, const b2FixtureDef& fixtureDef,
	const SharedPtr<b2Shape>& shape, float width, float height)
		: m_Dimension({width,height})
{
	PhysicsUserData data{};

	data.fixtureDef = fixtureDef;
	data.fixtureDef.userData.pointer = bodyDef.userData.pointer;
	data.bodyDef = bodyDef;
	data.shape = shape;

	const auto& [x, y] = m_PhysicsBody->GetPosition();

	m_Position.x = x;
	m_Position.y = y;
	
	world.AddPhysiscsBodyDef(data);
}

SLD::Box2DComponent::Box2DComponent(WorldEntity& world, b2BodyDef& bodyDef, float width, float height,
                                    float density)
	: m_Dimension({ width,height })
{

	const auto& physicsWorld{ world.GetPhysicsWorld() };

	const GameObjectId parentId{ bodyDef.userData.pointer };
	TransformComponent* transform{ world.GetComponent<TransformComponent>(parentId) };
	if (!transform)
		throw std::exception("need transform");

	const auto& worldPos{ transform->GetWorldPos() };
	bodyDef.position.x = worldPos.x;
	bodyDef.position.y = worldPos.y;

	b2PolygonShape shape{};
	shape.SetAsBox(width, height);



	m_PhysicsBody = SharedPtr<b2Body>{ physicsWorld->CreateBody(&bodyDef),[&world](b2Body* p)
	{
		if (p)
		{
			if (const auto& physicsWorld{world.GetPhysicsWorld()};
				physicsWorld)
			{
				physicsWorld->DestroyBody(p);
			}
		}
	} };

	const auto& [x, y] = m_PhysicsBody->GetPosition();

	m_Position.x = x;
	m_Position.y = y;
	
	b2Fixture* fixture{ m_PhysicsBody->CreateFixture(&shape, density) };
	fixture->GetUserData().pointer = bodyDef.userData.pointer;

	//PhysicsUserData data{ };
//data.fixtureDef.shape = &shape;
//data.fixtureDef.userData.pointer = bodyDef.userData.pointer;
//data.fixtureDef.density = density;
//data.bodyDef = bodyDef;

//world.AddPhysiscsBodyDef(data);
}

void SLD::Box2DComponent::SetPosition(const rtm::float2f& pos)
{
	m_Position = pos;
	m_RigidBodyState = State::Teleport;
}

void SLD::Box2DComponent::SetVelocity(const rtm::float2f& velocity)
{
	m_Velocity = velocity;

	m_RigidBodyState = State::Sweep;
	m_PhysicsBody->SetLinearVelocity({velocity.x,velocity.y});
}

const rtm::float2f& SLD::Box2DComponent::GetPosition() const
{
	return m_Position;
}

const rtm::float2f& SLD::Box2DComponent::GetVelocity() const
{
	return m_Velocity;
}

const b2Filter& SLD::Box2DComponent::GetContactFilter() const
{
	return m_PhysicsBody->GetFixtureList()->GetFilterData();
}

void SLD::Box2DComponent::SetContactFilter(const b2Filter& filter)
{
	// Only supports one fixture
	m_PhysicsBody->GetFixtureList()->SetFilterData(filter);
}

const rtm::float2f& SLD::Box2DComponent::GetDimension() const
{
	return m_Dimension;
}

const SharedPtr<b2Body>& SLD::Box2DComponent::GetBody() const
{
	return m_PhysicsBody;
}

void SLD::Box2DComponent::SetPhysicsBody(const SharedPtr<b2Body>& body)
{
	m_PhysicsBody = body;
}

SLD::Box2DComponent::State SLD::Box2DComponent::GetState() const
{
	return m_RigidBodyState;
}
