
#ifndef SLDFRAMEWORK_PHYSICSTOTRANSFORMUPDATE
#define SLDFRAMEWORK_PHYSICSTOTRANSFORMUPDATE

#include "../../System/SystemTemplate.h"
#include "../../Components/TransformComponent.h"
#include "../Components/Box2DComponent.h"

namespace SLD
{
	class PhysicsToTransformUpdate : public SystemTemplate<TransformComponent,Box2DComponent>
	{
	public:

		PhysicsToTransformUpdate(WorldEntity& world);
		void OnUpdate(GameObjectId gameObjectId, float deltaTime, TransformComponent*, Box2DComponent*) override;
	};
}


#endif
