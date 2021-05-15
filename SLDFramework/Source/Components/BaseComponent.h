
#ifndef SLDFRAMEWORK_BASECOMPONENT_H
#define SLDFRAMEWORK_BASECOMPONENT_H

#include "../Core/Base.h"

namespace SLD
{
	class BaseComponent
	{
	public:

		void MarkDestroy();
		
		BaseComponent() = default;
		virtual ~BaseComponent() = default;

	private:
		
		bool m_IsMarkedDestroy{};
	};
}


#endif
