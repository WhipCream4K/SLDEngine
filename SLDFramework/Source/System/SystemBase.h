
#ifndef SLD_FRAMEWORK_SYSTEMBASE_H
#define SLD_FRAMEWORK_SYSTEMBASE_H

//#include <array>
#include "ECSTypes.h"
#include "PipelineLayer.h"
#include "../Rendering/System/RenderObject.h"
//#include "../Components/BaseComponent.h"

#include <sstream>

//#include "../Core/WorldEntity.h"

namespace SLD
{
	class Window;
	class WorldEntity;
	class InputSettings;
	class SystemBase
	{
	public:

		SystemBase(WorldEntity& world, PipelineLayer layer);
		virtual ~SystemBase() = default;

		virtual void InternalUpdate(float deltaTime, const SharedPtr<Archetype>& archetype) = 0;
		virtual void InternalRender(const SharedPtr<Window>& winHandle, const SharedPtr<Archetype>& archetype) = 0;
		virtual void InternalRender(const SharedPtr<Window>&, std::vector<RenderObject>&) {}

		virtual void FetchFutureResults() {}
		
		const std::string& GetKey() const;
		PipelineLayer GetLayer() const;

	protected:

		PipelineLayer m_Layer;
		std::string m_Key;
		WorldEntity& m_World;
	};


}

#endif