
#ifndef SLDFRAMEWORK_GAMEOBJECT_H
#define SLDFRAMEWORK_GAMEOBJECT_H

#include "../Core/Base.h"
#include "../Components/TransformComponent.h"
#include "../Components/NonTickComponent.h"
#include "../Components/TickComponent.h"
#include "../Core/WorldEntity.h"

namespace SLD
{
	class GameObject final
	{
	public:

		GameObject(const RefPtr<WorldEntity>& world);

		[[nodiscard]] RefPtr<TransformComponent> GetTransform();

		template<typename ComponentType,
			typename = std::enable_if_t<std::is_base_of_v<BaseComponent, ComponentType>>>
			[[nodiscard]] RefPtr<ComponentType> GetComponent();

		[[nodiscard]] const std::vector<RefPtr<BaseComponent>>& GetAllComponents() const;

		template<typename ComponentType,
			typename = std::enable_if_t<std::is_base_of_v<BaseComponent, ComponentType>>,
		typename ...Args>
			[[nodiscard]] RefPtr<ComponentType> CreateComponent(Args&&... args);

		RefPtr<RenderingComponent> CreateRenderingComponent(size_t elemSize,uint32_t elemCnt);

	private:

		std::vector<RefPtr<BaseComponent>> m_ComponentTable;
		WeakPtr<WorldEntity> m_World;
		WeakPtr<TransformComponent> m_Transform;
	};

	template <typename ComponentType, typename>
	RefPtr<ComponentType> GameObject::GetComponent()
	{
		for (const auto& component : m_ComponentTable)
		{
			const auto& out{ std::static_pointer_cast<ComponentType>(component) };
			if (out)
				return out;
		}

		return nullptr;
	}

	template <typename ComponentType, typename, typename ... Args>
	RefPtr<ComponentType> GameObject::CreateComponent(Args&&... args)
	{
		auto world{ m_World.lock() };
		
		if constexpr (std::is_base_of_v<NonTickComponent,ComponentType>)
		{
			RefPtr<ComponentType> component{ world->AllocNonTickComponent<ComponentType>(std::forward<Args>(args)...) };
			return component;
		}

		if constexpr (std::is_base_of_v<TickComponent,ComponentType>)
		{
			RefPtr<ComponentType> component{ world->AllocTickComponent<ComponentType>(std::forward<Args>(args)...) };
			return component;
		}

		return nullptr;
	}
}

#endif
