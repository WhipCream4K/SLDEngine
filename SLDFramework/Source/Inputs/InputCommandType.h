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
		SharedPtr<EventHandler> callback{};
		//WeakPtr<EventHandler> callback{};
		InputEvent iEvent{ InputEvent::IE_None };
		WeakPtr<GameObject> referencePointer{};
	};

	struct AxisCommand
	{
		SharedPtr<DynamicDelegate<AxisCallbackType>> callback{};
		//WeakPtr<DynamicDelegate<AxisCallbackType>> callback{};
		WeakPtr<GameObject> referencePointer{};
	};
	
	using AxisCommandType = SharedPtr<DynamicDelegate<AxisCallbackType>>;
}

#endif

