
#ifndef SLDFRAMEWORK_BASECOMPONENT_H
#define SLDFRAMEWORK_BASECOMPONENT_H

#include "../Core/Base.h"
#include "../Core/ObservePtr.h"

namespace SLD
{
	class GameObject;
	class BaseComponent
	{
	public:

		void MarkDestroy();

		//BaseComponent(ObservePtr<BaseComponent> )
		virtual ~BaseComponent() = default;

		//template<typename T,
		//	typename = EnableIsBasedOf<BaseComponent, T>>
		//	T * CastTo();
	
	private:

		bool m_IsMarkedDestroy{};
	};
}


#endif
