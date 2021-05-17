
#ifndef SLDFRAMEWORK_GAMEOBJECT_H
#define SLDFRAMEWORK_GAMEOBJECT_H

#include "../Core/Base.h"
#include "../Components/NonTickComponent.h"
//#include "../Components/TickComponent.h"
#include "../Components/TransformComponent.h"
#include "../Core/WorldEntity.h"

namespace SLD
{
	class TransformComponent;
	class GameObject final
	{
	public:

		//GameObject(const RefPtr<WorldEntity>& world);
		GameObject(WorldEntity& world);

		//GameObject(const RefPtr<const WorldEntity>& world);

		[[nodiscard]] const RefPtr<ObservePtr<TransformComponent>>& GetTransform() const;

		template<typename ComponentType,
			typename = std::enable_if_t<std::is_base_of_v<BaseComponent, ComponentType>>>
			[[nodiscard]] RefPtr<ComponentType> GetComponent();

		[[nodiscard]] const std::vector<RefPtr<BaseComponent>>& GetAllComponents() const;

		template<typename ComponentType,
			typename = EnableIsBasedOf<BaseComponent, ComponentType>,
			typename ...Args>
			[[nodiscard]] RefPtr<ComponentType> CreateComponent(Args&&... args);

		RefPtr<RenderingComponent> CreateRenderingComponent(size_t elemSize, uint32_t elemCnt);

		RefWrap<WorldEntity> GetWorld();

		GameObject(const GameObject& other);
		GameObject& operator=(const GameObject& other);
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(GameObject&& other) noexcept;
		~GameObject();

	private:

		std::vector<RefPtr<BaseComponent>> m_ComponentTable;
		RefWrap<WorldEntity> m_World;
		RefPtr<ObservePtr<TransformComponent>> m_Transform;
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
		//auto world{ m_World.lock() };

		if constexpr (std::is_base_of_v<NonTickComponent, ComponentType>)
		{
			RefPtr<ComponentType> component{ m_World->AllocNonTickComponent<ComponentType>(std::forward<Args>(args)...) };
			return component;
		}

		if constexpr (std::is_base_of_v<TickComponent, ComponentType>)
		{
			RefPtr<ComponentType> component{ m_World->AllocTickComponent<ComponentType>(std::forward<Args>(args)...) };
			return component;
		}

		return nullptr;
	}
}

#endif
