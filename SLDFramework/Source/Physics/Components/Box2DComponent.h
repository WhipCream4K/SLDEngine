
#ifndef SLDFRAMEWORK_PHYSICSCOMPONENT_H
#define SLDFRAMEWORK_PHYSICSCOMPONENT_H

#include "../../Core/Base.h"
#include "../../Components/BaseComponent.h"
#include <box2d/box2d.h>

class b2Shape;
struct b2BodyDef;
struct b2FixtureDef;
class b2Body;

namespace SLD
{
	class TransformComponent;
	class WorldEntity;
	class Box2DComponent : public ComponentT<Box2DComponent>
	{

	public:

		enum class State
		{
			Sweep,
			Teleport
		};

		Box2DComponent(
			WorldEntity& world,
			const b2BodyDef& bodyDef,
			const b2Shape& shape,
			float density = 1.0f);

		Box2DComponent(
			WorldEntity& world,
			const b2BodyDef& bodyDef,
			const b2FixtureDef& fixtureDef,
			float width,
			float height
		);

		Box2DComponent(
			WorldEntity& world,
			const b2BodyDef& bodyDef,
			const b2FixtureDef& fixtureDef,
			const SharedPtr<b2Shape>& shape,
			float width,
			float height
		);

		Box2DComponent(
			WorldEntity& world,
			b2BodyDef& bodyDef,
			float width,
			float height,
			float density = 1.0f
		);

		void SetPosition(const rtm::float2f& pos);
		void SetVelocity(const rtm::float2f& velocity);

		const rtm::float2f& GetPosition() const;
		const rtm::float2f& GetVelocity() const;

		const b2Filter& GetContactFilter() const;
		void SetContactFilter(const b2Filter& filter);

		const rtm::float2f& GetDimension() const;
		[[nodiscard]] const SharedPtr<b2Body>& GetBody() const;
		void SetPhysicsBody(const SharedPtr<b2Body>& body);
		State GetState() const;

	private:

		SharedPtr<b2Body> m_PhysicsBody;
		rtm::float2f m_Dimension;
		rtm::float2f m_Velocity{};
		rtm::float2f m_Position{};

		// NOTE: Box2d supports 16 bits mask
		uint16_t m_ContactFilter{};
		State m_RigidBodyState{ State::Sweep };
	};
}


#endif
