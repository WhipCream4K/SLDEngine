
#ifndef SLDFRAMEWORK_CONTACTFILTER_H
#define SLDFRAMEWORK_CONTACTFILTER_H

//#include "../../Core/Base.h"
#include <box2d/box2d.h>

namespace SLD
{
	class WorldEntity;
	class ContactFilter : public b2ContactFilter
	{
	public:
		
		ContactFilter(WorldEntity& world);
		bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
	};
}


#endif
