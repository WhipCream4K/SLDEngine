
#ifndef SLDFRAMEWORK_BASECOMPONENT_H
#define SLDFRAMEWORK_BASECOMPONENT_H

#include "../Core/Base.h"
#include "../Core/ObservePtr.h"

namespace SLD
{
	class GameObject;
	class BaseComponent
	{
		friend class GameObject;
	
	public:
		
		void MarkDestroy();
		virtual ~BaseComponent() = default;
		
		//template<typename T,
		//	typename = EnableIsBasedOf<BaseComponent, T>>
		//	T * CastTo();

	protected:

		WeakPtr<GameObject> GetParent() const noexcept;
	
	private:

		// Could move this to constructor of object
		// but then every constructor of every component
		// and it's kinda annoying
		void SetParent(const RefPtr<GameObject>& parent);
		
	private:

		WeakPtr<GameObject> m_Parent;
		bool m_IsMarkedDestroy{};
	};
}


#endif
