
#ifndef SLDFRAMEWORK_ASYNCTRANSFORMUPDATE_H
#define SLDFRAMEWORK_ASYNCTRANSFORMUPDATE_H

#include "SystemTemplate.h"
#include "../Components/TransformComponent.h"

namespace SLD
{
	class AsyncTransformUpdate : public SystemTemplate<TransformComponent>
	{
	public:
		AsyncTransformUpdate(WorldEntity& world);
		void OnUpdate(GameObjectId gameObjectId, float deltaTime, TransformComponent*) override;
	};
}


#endif


