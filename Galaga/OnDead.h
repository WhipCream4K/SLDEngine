#pragma once

#include <Components/BaseComponent.h>

class OnDead : public SLD::ComponentT<OnDead>
{
public:
	
	OnDead();
	virtual void CallOnDead() = 0;
	
};
