#pragma once

#include <SLDFramework.h>

class OnObjectDead : public SLD::ComponentT<OnObjectDead>
{
public:

	virtual ~OnObjectDead() = default;
	virtual void OnDead(SLD::WorldEntity& , SLD::GameObjectId ) {}
};

