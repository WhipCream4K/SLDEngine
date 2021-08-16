
#ifndef SLDFRAMEWORK_PHYSICSCALLBACK_H
#define SLDFRAMEWORK_PHYSICSCALLBACK_H

#include "../../System/SystemTemplate.h"
#include "../../Components/TransformComponent.h"
#include "../Components/Box2DComponent.h"
#include <box2d/box2d.h>

namespace SLD
{
	template<typename ...ComponentTypes>
	class PhysicsCallback :
		public SystemTemplate<TransformComponent, Box2DComponent, ComponentTypes...>,
		public b2ContactListener
	{
	public:

		PhysicsCallback(WorldEntity& world);
		virtual ~PhysicsCallback() override = default;
		virtual void OnBeginOverlap(GameObjectId collider, GameObjectId against, b2Contact* contactPoint) = 0;
	
	private:
		
		virtual void BeginContact(b2Contact* contact) override;
	};

	template <typename ... ComponentTypes>
	PhysicsCallback<ComponentTypes...>::PhysicsCallback(WorldEntity& world)
		: SystemTemplate<TransformComponent,Box2DComponent,ComponentTypes...>(world,PipelineLayer::OnPreAsync)
		, b2ContactListener()
	{
		//world.GetPhysicsWorld()->SetContactListener(this);
	}

	template <typename ... ComponentTypes>
	void PhysicsCallback<ComponentTypes...>::BeginContact(b2Contact* contact)
	{
		WorldEntity& world{ this->m_World };
		const size_t contactItemCnt{ 2 };
		const GameObjectId id_a{ contact->GetFixtureA()->GetUserData().pointer };
		const GameObjectId id_b{ contact->GetFixtureB()->GetUserData().pointer };

		if (id_a == id_b)
			return;
		
		// if either a or b has the same archetype of this system then invoke begin overlap
		bool isExist{};
		size_t contactAgainst{};
		size_t collider{};
		
		for (size_t i = 0; i < contactItemCnt; ++i)
		{
			const GameObjectId id{ i == 0 ? id_a : id_b };
			const SharedPtr<Archetype> archetype{ world.GetGameObjectRecord(id).restedArchetype.lock() };
			
			if (WorldEntity::IsArchetypeIdsSameAsKey(
				this->m_Key,
				WorldEntity::ArchetypeIdsToKey(archetype->types)))
			{
				isExist = true;
				contactAgainst = id == id_a ? id_b : id_a;
				collider = id;
				break;
			}
		}

		if(isExist)
		{
			OnBeginOverlap(collider, contactAgainst, contact);
		}
	}
}


#endif
