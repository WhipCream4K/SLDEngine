#pragma once

#include <SLDFramework.h>

class OnEnemyDead : public SLD::ListenerT<SLD::GameObjectId>
{
public:

	OnEnemyDead() = default;
};