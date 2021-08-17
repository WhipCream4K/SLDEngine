
#ifndef SLDFRAMEWORK_WORLDTOPHYSICSUPDATE
#define SLDFRAMEWORK_WORLDTOPHYSICSUPDATE

#include "../../System/SystemTemplate.h"
#include "../Components/Box2DComponent.h"
#include "../../Components/TransformComponent.h"

namespace SLD
{
	class WorldToPhysicsUpdate : public SystemTemplate<TransformComponent,Box2DComponent>
	{
	public:

		WorldToPhysicsUpdate(WorldEntity& world);

		void OnUpdate(GameObjectId gameObjectId, float deltaTime, TransformComponent*, Box2DComponent*) override;
	};
}


#endif
