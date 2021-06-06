#ifndef SLDFRAMEWORK_INPUTCOMMANDTYPE_H
#define SLDFRAMEWORK_INPUTCOMMANDTYPE_H

#include "InputManager.h"
#include "../Core/Event.h"
#include "../Core/DynamicDelegate.h"

namespace SLD
{
	class GameObject;
	class InputComponent;
	using AxisCallbackType = void(float);
	
	struct ActionCommand
	{
		RefPtr<EventHandler> callback{};
		//WeakPtr<EventHandler> callback{};
		InputEvent iEvent{ InputEvent::IE_None };
		WeakPtr<GameObject> referencePointer{};
	};

	struct AxisCommand
	{
		RefPtr<DynamicDelegate<AxisCallbackType>> callback{};
		//WeakPtr<DynamicDelegate<AxisCallbackType>> callback{};
		WeakPtr<GameObject> referencePointer{};
	};
	
	using AxisCommandType = RefPtr<DynamicDelegate<AxisCallbackType>>;
}

#endif

