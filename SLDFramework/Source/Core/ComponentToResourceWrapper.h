
#ifndef SLDFRAMEWORK_COMPONENT_TO_RESOURCE_WRAPPER
#define SLDFRAMEWORK_COMPONENT_TO_RESOURCE_WRAPPER

#include "Base.h"

namespace SLD
{
	class BaseComponent;
	struct ComponentToResourceWrapper final
	{
		ComponentToResourceWrapper(ObservePtr ptr)
			: ptrToResource(std::move(ptr))
		{
		}

		ComponentToResourceWrapper(std::add_pointer_t<void>& bufferHead,size_t offset)
			: ptrToResource(bufferHead,offset)
		{
		}
		
		ObservePtr ptrToResource;
		RefPtr<BaseComponent> component{};
	};

}

#endif
