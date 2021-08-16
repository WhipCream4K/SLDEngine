#include "PhysicsCallback.h"

//SLD::PhysicsCallback::PhysicsCallback(WorldEntity& world)
//	: SystemTemplate(world,PipelineLayer::OnPhysicsValidation)
//{
//	world.GetPhysicsWorld()->SetContactListener(this);
//}
//
//void SLD::PhysicsCallback::BeginContact(b2Contact* contact)
//{
//	const GameObjectId firstId{ contact->GetFixtureA()->GetUserData().pointer };
//	const GameObjectId secondId{ contact->GetFixtureB()->GetUserData().pointer };
//
//	const auto firstArchetype{ m_World.GetGameObjectRecord(firstId).restedArchetype.lock() };
//	
//	OnBeginOverlap(firstId, secondId,contact);
//}
