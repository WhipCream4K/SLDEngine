
#ifndef SLDFRAMEWORK_GAMEOBJECT_H
#define SLDFRAMEWORK_GAMEOBJECT_H

#include "../Core/Base.h"
#include "../Core/WorldEntity.h"
//#include "../Components/RenderingComponent.h"

namespace SLD
{
	class RenderingComponent;
	class BaseComponent;
	class TransformComponent;
	class WorldEntity;
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		
		GameObject(WorldEntity& world);


		static SharedPtr<GameObject> Create(SLD::WorldEntity& world);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* GetComponent();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* AddComponent(ComponentType&& val);

		template<typename ComponentType, typename ...Args>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* AddComponent(Args&&... args);

#pragma region deprecated
		
		[[nodiscard]] const SharedPtr<ObservePtr<TransformComponent>>& GetTransform() const;
		template<typename ComponentType,
			typename = EnableIsBasedOf<BaseComponent, ComponentType>,
			typename ...Args>
			[[nodiscard]] WeakPtr<ObservePtr<ComponentType>> CreateComponent(Args&&... args);

		WeakPtr<ObservePtr<RenderingComponent>> CreateRenderingComponent(size_t elemSize, uint32_t elemCnt);
		
		
#pragma endregion 
		
		GameObjectId GetId() const;

		WorldEntity& GetWorld();

		GameObject(const GameObject& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(GameObject&& other) noexcept;
		~GameObject();

	private:


		template<typename T, typename ...Args>
		T* AllocComponentInArchetype(const Record& gameObjectRec, const SharedPtr<Archetype>& archetype, Args&&... args);

		void RemoveGameObjectFromArchetype(const Record& oldRecord, GameObjectId id);

	private:
		
		GameObjectId m_Id;
		WorldEntity& m_World;

#pragma region deprecated
		SharedPtr<ObservePtr<TransformComponent>> m_Transform;
#pragma endregion 
	};

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* GameObject::GetComponent()
	{
		return m_World.GetComponent<ComponentType>(m_Id);
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* GameObject::AddComponent(ComponentType&& val)
	{
		const ComponentTypeId newComponentTypeId{ ComponentT<ComponentType>::GetId() };

		Record& record{ m_World.GetGameObjectRecord(m_Id) };

		SharedPtr<Archetype> oldArchetype{ record.restedArchetype.lock() };

		SharedPtr<Archetype> destinedArchetype{};

		ComponentType* out{};

		bool shouldRemoveGameObject{};
		bool shouldAddGameObjectToWaitList{};

		// 1. check this GameObject already exist in any archetype
		if (oldArchetype)
		{
			// 1.1 if so check if this component is already in register in this archetype
			if (std::find(oldArchetype->types.begin(),
				oldArchetype->types.end(),
				newComponentTypeId) != oldArchetype->types.end())
			{
				return nullptr;
			}

			// 1.2 if not then combined the ComponentId to ArchetypeId
			// and create or get an archetype of the ComponentId

			// assume 1:2:4
			std::vector<ComponentTypeId> temp{ oldArchetype->types };

			// becomes 1:2:4:3
			temp.emplace_back(newComponentTypeId);

			// becomes 1:2:3:4
			std::sort(temp.begin(), temp.end());

			// 1.3 Create new copy of archetype to be inserted in pending validate archetype
			SharedPtr<Archetype> newArchetype{};

			// 1.4 Check if there's an existing archetype without automatically creating it
			//
			auto [isExist, archetypeInstance] {m_World.GetArchetype(temp)};

			newArchetype = archetypeInstance.lock();

			if (!isExist)
			{
				// 1.4.1 Register size of this new component
				const size_t newComponentSize{
					m_World.GetComponentSizeAndConstruct<ComponentType>() };

				newArchetype->types = temp;
				newArchetype->sizeOfStruct = oldArchetype->sizeOfStruct + int(newComponentSize);
			}
			else
			{
				// if it exist before we might need to store it in case of archetype iteration
				shouldAddGameObjectToWaitList = true;
			}

			destinedArchetype = newArchetype;
			shouldRemoveGameObject = true;
		}
		else
		{
			// 2. if this is the first component of this GameObject then
			// create or get an archetype of this componentId
			SharedPtr<Archetype> newArchetype{};

			auto [isExist, archetypeInstance] {m_World.GetArchetype({ newComponentTypeId })};

			newArchetype = archetypeInstance.lock();

			if (!isExist)
			{
				const size_t newComponentSize{
					m_World.GetComponentSizeAndConstruct<ComponentType>() };

				newArchetype->types = { newComponentTypeId };
				newArchetype->sizeOfStruct = int(newComponentSize);
			}
			else
			{
				// if it exist before we might need to store it in case of archetype iteration
				shouldAddGameObjectToWaitList = true;
			}

			destinedArchetype = newArchetype;
		}

		// 3. Create Component to the destinedArchetype
		out = AllocComponentInArchetype<ComponentType>(record, destinedArchetype, std::forward<ComponentType>(val));

		if (shouldRemoveGameObject)
			RemoveGameObjectFromArchetype(record, m_Id);

		// 4. Add entity to old archetype
		//if (shouldAddGameObjectToWaitList)
		//{
		//	m_World.AddGameObjectToWaitingList(m_Id);
		//	record.componentStructIdx = int(destinedArchetype->numOfGameObjects++);
		//}
		//else
		//{
		destinedArchetype->gameObjectIds.emplace_back(m_Id); // bad
		//	record.componentStructIdx = int(destinedArchetype->gameObjectIds.size()) - 1;
		//	++destinedArchetype->numOfGameObjects;
		//}

		// 5. Assign this GameObject to record
		record.componentStructIdx = int(destinedArchetype->gameObjectIds.size()) - 1;
		record.restedArchetype = destinedArchetype;
		shouldAddGameObjectToWaitList;
		m_World.MarkArchetypeMapDirty();

		return out;
	}

	template <typename ComponentType, typename ... Args>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* GameObject::AddComponent(Args&&... args)
	{

		const ComponentTypeId newComponentTypeId{ ComponentT<ComponentType>::GetId() };

		Record& record{ m_World.GetGameObjectRecord(m_Id) };

		SharedPtr<Archetype> oldArchetype{ record.restedArchetype.lock() };

		SharedPtr<Archetype> destinedArchetype{};

		ComponentType* out{};

		bool shouldRemoveGameObject{};
		bool shouldAddGameObjectToWaitList{};

		// 1. check this GameObject already exist in any archetype
		if (oldArchetype)
		{
			// 1.1 if so check if this component is already in register in this archetype
			if (std::find(oldArchetype->types.begin(),
				oldArchetype->types.end(),
				newComponentTypeId) != oldArchetype->types.end())
			{
				return nullptr;
			}

			// 1.2 if not then combined the ComponentId to ArchetypeId
			// and create or get an archetype of the ComponentId

			// assume 1:2:4
			std::vector<ComponentTypeId> temp{ oldArchetype->types };

			// becomes 1:2:4:3
			temp.emplace_back(newComponentTypeId);

			// becomes 1:2:3:4
			std::sort(temp.begin(), temp.end());

			// 1.3 Create new copy of archetype to be inserted in pending validate archetype
			SharedPtr<Archetype> newArchetype{};

			// 1.4 Check if there's an existing archetype without automatically creating it
			//
			auto [isExist, archetypeInstance] {m_World.GetArchetype(temp)};

			newArchetype = archetypeInstance.lock();

			if (!isExist)
			{
				// 1.4.1 Register size of this new component
				const size_t newComponentSize{
					m_World.GetComponentSizeAndConstruct<ComponentType>() };

				newArchetype->types = temp;
				newArchetype->sizeOfStruct = oldArchetype->sizeOfStruct + int(newComponentSize);
			}
			else
			{
				// if it exist before we might need to store it in case of archetype iteration
				shouldAddGameObjectToWaitList = true;
			}

			destinedArchetype = newArchetype;
			shouldRemoveGameObject = true;
		}
		else
		{
			// 2. if this is the first component of this GameObject then
			// create or get an archetype of this componentId
			SharedPtr<Archetype> newArchetype{};

			auto [isExist, archetypeInstance] {m_World.GetArchetype({ newComponentTypeId })};

			newArchetype = archetypeInstance.lock();

			if (!isExist)
			{
				const size_t newComponentSize{
					m_World.GetComponentSizeAndConstruct<ComponentType>() };

				newArchetype->types = { newComponentTypeId };
				newArchetype->sizeOfStruct = int(newComponentSize);
			}
			else
			{
				// if it exist before we might need to store it in case of archetype iteration
				shouldAddGameObjectToWaitList = true;
			}

			destinedArchetype = newArchetype;
		}

		// 3. Create Component to the destinedArchetype
		out = AllocComponentInArchetype<ComponentType>(record, destinedArchetype, std::forward<Args>(args)...);

		if (shouldRemoveGameObject)
			RemoveGameObjectFromArchetype(record, m_Id);

		// 4. Add entity to new archetype
		//if (shouldAddGameObjectToWaitList)
		//{
		//	m_World.AddGameObjectToWaitingList(m_Id);
		//	record.componentStructIdx = int(destinedArchetype->numOfGameObjects++);
		//}
		//else
		//{
		destinedArchetype->gameObjectIds.emplace_back(m_Id); // bad
		//	record.componentStructIdx = int(destinedArchetype->gameObjectIds.size()) - 1;
		//	++destinedArchetype->numOfGameObjects;
		//}

		// 5. Assign this GameObject to record
		record.componentStructIdx = int(destinedArchetype->gameObjectIds.size()) - 1;
		record.restedArchetype = destinedArchetype;
		shouldAddGameObjectToWaitList;
		m_World.MarkArchetypeMapDirty();

		return out;
	}

	template <typename ComponentType, typename, typename ... Args>
	WeakPtr<ObservePtr<ComponentType>> GameObject::CreateComponent(Args&&...)
	{
		//SharedPtr<ObservePtr<ComponentType>> component{};

		//if constexpr (std::is_same_v<InputComponent, ComponentType>)
		//	component = m_World.AllocNonTickComponent<ComponentType>(shared_from_this());

		////else if constexpr (std::is_same_v<Rendering2Component, ComponentType>)


		//else if constexpr (std::is_base_of_v<TickComponent, ComponentType>)
		//	component = m_World.AllocTickComponent<ComponentType>(std::forward<Args>(args)...);

		//else if constexpr (std::is_base_of_v<NonTickComponent, ComponentType>)
		//	component = m_World.AllocNonTickComponent<ComponentType>(std::forward<Args>(args)...);


		//if (component)
		//{
		//	auto base{ reinterpret_cast<SharedPtr<ObservePtr<BaseComponent>>&>(component) };
		//	base->GetPtr()->SetParent(shared_from_this());
		//	m_ComponentTable.emplace_back(base);
		//}

		//return component;
		return {};
	}

	template <typename T, typename ... Args>
	T* GameObject::AllocComponentInArchetype(const Record& gameObjectRec, const SharedPtr<Archetype>& archetype,
		Args&&... args)
	{
		constexpr size_t sizeOfNewComponent{ sizeof(T) };
		const size_t newComponentId{ ComponentT<T>::GetId() };

		T* out{};

		// 2. Data Creation
		SharedPtr<uint8_t[]> newComponentArray{ archetype->componentData };

		const int newOccupiedComponentArraySize{ archetype->sizeOfStruct * int(archetype->gameObjectIds.size()) };
		//const int newOccupiedComponentArraySize{ archetype->sizeOfStruct * int(archetype->numOfGameObjects) };
		const int newComponentArraySize{ archetype->totalLength };

		if (newOccupiedComponentArraySize + int(sizeOfNewComponent) >
			newComponentArraySize || !archetype->componentData)
		{
			const int newSize{ newComponentArraySize + (archetype->sizeOfStruct * 3) };
			newComponentArray = SharedPtr<uint8_t[]>{ new uint8_t[newSize]{} };

			if (archetype->componentData)
			{
				// Copy from old resource
				std::copy_n(
					archetype->componentData.get(),
					newOccupiedComponentArraySize,
					newComponentArray.get());
			}

			// Set New data
			archetype->totalLength = newSize;
			archetype->componentData = newComponentArray;
		}

		uint8_t* newComponentStartAddress{ &newComponentArray[newOccupiedComponentArraySize] };

		// 3. if old data address exist then copy over those data too
		if (auto checkPtr{ gameObjectRec.restedArchetype.lock() };
			checkPtr)
		{
			// to save the offset when iterate
			size_t thisComponentOffsetFromStruct{};

			// 3.1 reorganized component structure
			// for the case that the types is not sorted before
			uint8_t* oldDataAddress{ &checkPtr->componentData[gameObjectRec.componentStructIdx * checkPtr->sizeOfStruct] };
			uint8_t* newDataAddress{ &newComponentArray[newOccupiedComponentArraySize] };


			size_t oldOffset{};
			for (const auto& oldId : checkPtr->types)
			{
				const auto fIter{ std::find(archetype->types.begin(), archetype->types.end(), oldId) };

				if (fIter != archetype->types.end())
				{
					size_t offset{};
					std::for_each(archetype->types.begin(), fIter, [this, &offset](const ComponentTypeId id)
						{
							offset += m_World.GetComponentSizeById(id);
						});

					uint8_t* copyStart{ oldDataAddress + oldOffset };
					uint8_t* copyEnd{ newDataAddress + offset };

					m_World.GetRegisterComponent(oldId)->MoveData(copyStart, copyEnd);
				}

				oldOffset += m_World.GetComponentSizeById(oldId);

			}

			for (const auto& type : archetype->types)
			{
				if (type == newComponentId)
					break;

				thisComponentOffsetFromStruct += m_World.GetComponentSizeById(type);
			}

			newComponentStartAddress += thisComponentOffsetFromStruct;
		}



		out = new (newComponentStartAddress) T{ std::forward<Args>(args)... };
		return out;
	}
}

#endif
