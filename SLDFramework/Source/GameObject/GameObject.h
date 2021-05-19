
#ifndef SLDFRAMEWORK_GAMEOBJECT_H
#define SLDFRAMEWORK_GAMEOBJECT_H

#include "../Core/Base.h"
#include "../Components/NonTickComponent.h"
#include "../Components/TransformComponent.h"
#include "../Core/WorldEntity.h"

namespace SLD
{
	class TransformComponent;

	//struct ComponentPointerWrapper
	//{
	//	ComponentPointerWrapper(ObservePtr ptr)
	//		: resourcePointer(ptr)
	//	{
	//	}
	//	
	//	ObservePtr resourcePointer;
	//	RefPtr<BaseComponent> component;
	//};
	
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		//GameObject(const RefPtr<WorldEntity>& world);
		GameObject(WorldEntity& world);

		//GameObject(const RefPtr<const WorldEntity>& world);

		[[nodiscard]] const RefPtr<ObservePtr<TransformComponent>>& GetTransform() const;

		template<typename ComponentType,
			typename = EnableIsBasedOf<BaseComponent,ComponentType>>
			[[nodiscard]] WeakPtr<ObservePtr<ComponentType>> GetComponent();


		template<typename ComponentType,
			typename = EnableIsBasedOf<BaseComponent, ComponentType>,
			typename ...Args>
			[[nodiscard]] WeakPtr<ObservePtr<ComponentType>> CreateComponent(Args&&... args);

		RefPtr<RenderingComponent> CreateRenderingComponent(size_t elemSize, uint32_t elemCnt);

		RefWrap<WorldEntity> GetWorld();

		GameObject(const GameObject& other);
		GameObject& operator=(const GameObject& other);
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(GameObject&& other) noexcept;
		~GameObject();

	private:

		std::vector<RefPtr<ObservePtr<BaseComponent>>> m_ComponentTable;
		RefWrap<WorldEntity> m_World;
		RefPtr<ObservePtr<TransformComponent>> m_Transform;
	};

	template <typename ComponentType, typename>
	WeakPtr<ObservePtr<ComponentType>> GameObject::GetComponent()
	{
		for (auto& component : m_ComponentTable)
		{
			if (dynamic_cast<ComponentType*>(component->GetPtr()))
				return reinterpret_cast<RefPtr<ObservePtr<ComponentType>>&>(component);
		}

		return {};
	}

	template <typename ComponentType, typename, typename ... Args>
	WeakPtr<ObservePtr<ComponentType>> GameObject::CreateComponent(Args&&... args)
	{
		// TODO: Fix me
		
		RefPtr<ObservePtr<ComponentType>> component{};

		if constexpr (std::is_same_v<InputComponent, ComponentType>)
			component = m_World.get().AllocNonTickComponent<ComponentType>(shared_from_this());
		
		else if constexpr (std::is_base_of_v<TickComponent,ComponentType>)
			component = m_World.get().AllocTickComponent<ComponentType>(std::forward<Args>(args)...);
		
		else if constexpr (std::is_base_of_v<NonTickComponent,ComponentType>)
			component = m_World.get().AllocNonTickComponent<ComponentType>(std::forward<Args>(args)...);

		if(component)
			m_ComponentTable.emplace_back(reinterpret_cast<RefPtr<ObservePtr<BaseComponent>>&>(component));
		
		return component;
	}

	//template <typename ComponentType, typename, typename ... Args>
	//RefPtr<ComponentType> GameObject::CreateComponent(Args&&... args)
	//{
	//	//auto world{ m_World.lock() };

	//	if constexpr (std::is_base_of_v<NonTickComponent, ComponentType>)
	//	{
	//		RefPtr<ComponentType> component{ m_World->AllocNonTickComponent<ComponentType>(std::forward<Args>(args)...) };
	//		return component;
	//	}

	//	if constexpr (std::is_base_of_v<TickComponent, ComponentType>)
	//	{
	//		RefPtr<ComponentType> component{ m_World->AllocTickComponent<ComponentType>(std::forward<Args>(args)...) };
	//		return component;
	//	}

	//	return nullptr;
	//}
}

#endif
