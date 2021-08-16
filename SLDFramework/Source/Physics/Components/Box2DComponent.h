
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

		const b2Filter& GetContactFilter() const;
		void SetContactFilter(const b2Filter& filter);

		const rtm::float2f& GetDimension() const;
		[[nodiscard]] const SharedPtr<b2Body>& GetBody() const;
		void SetPhysicsBody(const SharedPtr<b2Body>& body);
	
	private:

		SharedPtr<b2Body> m_PhysicsBody;
		rtm::float2f m_Dimension;
		
		// NOTE: Box2d supports 16 bits mask
		uint16_t m_ContactFilter{};
	};
}


#endif
