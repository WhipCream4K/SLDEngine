#ifndef SLDFRAMEWORK_INPUTCOMMANDTYPE_H
#define SLDFRAMEWORK_INPUTCOMMANDTYPE_H

#include "InputManager.h"
#include "../Core/Event.h"
#include "../Core/VirMemDelegate.h"

namespace SLD
{
	using AxisCallbackType = void(float);
	
	struct ActionCommandType
	{
		RefPtr<EventHandler> callback{};
		InputEvent iEvent{ InputEvent::IE_None };
	};
	
	using AxisCommandType = RefPtr<VirMemDelegate<AxisCallbackType>>;
}

#endif

