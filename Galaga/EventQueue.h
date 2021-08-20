#pragma once

#include <Components/BaseComponent.h>
#include "GameEvent.h"

struct EventQueue : SLD::ComponentT<EventQueue>
{
	EventQueue();

	//std::unordered_set<std::string> eventHappend;
};