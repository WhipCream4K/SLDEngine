
#ifndef SLDFRAMEWORK_TICKCOMPONENT_H
#define SLDFRAMEWORK_TICKCOMPONENT_H

#include "../Core/Base.h"
#include "BaseComponent.h"
#include "../Core/ScriptableChecks.h"


namespace SLD
{
	
	class TickComponent : public BaseComponent
	{
	public:

		enum class Type
		{
			TransformComponent,

			Count // Use to keep track of all the tick component
		};

		virtual void AsyncUpdate([[maybe_unused]] float) {}

		virtual ~TickComponent() override = default;
	};
}

#endif
