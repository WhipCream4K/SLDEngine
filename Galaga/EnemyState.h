#pragma once

#include <SLDFramework.h>
#include "MyComponents.h"

class FlyInState;

class EnemyState
{
public:

	static SharedPtr<FlyInState> FlyInState;
	
	virtual ~EnemyState() = default;
	
	virtual SharedPtr<EnemyState> Update(
		SLDWorldEntity&,
		SLD::GameObjectId
	);
	
	virtual void Enter(SLDWorldEntity&, SLD::GameObjectId) {}
	virtual void Exit(SLD::WorldEntity&, SLD::GameObjectId) {}
};



