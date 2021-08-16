#include "ContactFilter.h"
#include "../../Core/WorldEntity.h"

SLD::ContactFilter::ContactFilter(WorldEntity& world)
{
	world.GetPhysicsWorld()->SetContactFilter(this);
}

bool SLD::ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	// NOTE: Follows default implementation of Box2D
	// https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md112

	const b2Filter& filterA{ fixtureA->GetFilterData() };
	const b2Filter& filterB{ fixtureB->GetFilterData() };

	//if(filterA.groupIndex == filterB.groupIndex &&
	//	filterA.groupIndex != 0)
	//{
	//	return filterA.groupIndex > 0;
	//}

	const bool collideA{ (filterA.maskBits & filterB.categoryBits) != 0 };
	const bool collideB{ (filterA.categoryBits & filterB.maskBits) != 0 };
	bool shouldCollide{ collideA && collideB };
	return shouldCollide;
}
