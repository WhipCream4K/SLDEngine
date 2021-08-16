
#ifndef SLDFRAMEWORK_PREFAB
#define SLDFRAMEWORK_PREFAB

#include "../Core/Base.h"

namespace SLD
{
	class GameObject;
	class Prefab
	{
	public:

		virtual void OnCreate(const SharedPtr<GameObject>& gameObject) = 0;
		
		virtual ~Prefab() = default;
	};
}

#endif
