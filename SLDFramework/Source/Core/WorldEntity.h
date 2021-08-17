#ifndef SLDFRAMEWORK_WORLDENTITY_H
#define SLDFRAMEWORK_WORLDENTITY_H

#include "Base.h"
#include "PMR_Resource/LoggingResource.h"
#include "PMR_Resource/PoolResource.h"
#include "../Inputs/InputSettings.h"
#include "../Components/TickComponent.h"
#include "PersistentThreadWorker.h"
#include "PMR_Resource/UnSynchronizedSpatialResource.h"
#include "../Rendering/RenderBuffer.h"
#include "../System/SystemBase.h"
#include "Thread/SimpleThreadPool.h"
#include <box2d/box2d.h>
#include "../Physics/Components/PhysicsUserData.h"

class b2World;
namespace SLD
{
	class ContactFilter;
	class Prefab;
	// For support both 32 and 64 bit
	using GameObjectId = size_t;

	struct ComponentRecord
	{
		SharedPtr<BaseComponent> instance{};
		size_t size{};
	};

	struct MoveComponentRecord
	{
		GameObjectId id{};
		size_t oldCompIndex{};
		size_t componentId{};
		SharedPtr<Archetype> oldArchetype{};
	};
	
	enum ObjectBuffer
	{
		Write,
		Copy,
		Read,

		Count
	};

	class RenderingComponent;
	class NonTickComponent;
	class TransformComponent;
	class Rendering2Component;
	class WorldEntity final
	{
	public:


		WorldEntity();
		~WorldEntity();

		// Won't return anything because vector loses reference when moved
		//SharedPtr<RenderingComponent> AllocRenderComponent(const SharedPtr<ObservePtr<TransformComponent>>& transform, size_t elemSize, uint32_t elemCnt);

		RenderBuffer& GetRenderBuffer();

		SharedPtr<ObservePtr<RenderingComponent>> AllocRenderingComponent(
			const SharedPtr<ObservePtr<TransformComponent>>& transform,
			size_t elemSize,
			uint32_t elemCnt);

		uint8_t* GetRenderDataBufferStart() const noexcept;
		uint32_t GetRenderElementCount() const noexcept;
		size_t GetRenderDataSize() const noexcept;

		template<typename ComponentType,
			typename = EnableIsBasedOf<TickComponent, ComponentType >,
			typename ...Args>
			SharedPtr<ObservePtr<ComponentType>> AllocTickComponent(Args&&... args);

		float GetDeltaTime() const;

		template<typename ComponentType,
			typename = EnableIsBasedOf<NonTickComponent, ComponentType>,
			typename ...Args>
			SharedPtr<ObservePtr<ComponentType>> AllocNonTickComponent(Args&&... args);

		[[nodiscard]] InputSettings& GetWorldInputSetting();

		/// <summary>
		/// return pointer to that component from the given GameObject Id
		/// </summary>
		/// <typeparam name="ComponentType"></typeparam>
		/// <param name="id">GameObject Id</param>
		/// <returns>pointer to component</returns>
		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* GetComponent(GameObjectId id);

		template<typename ComponentType, typename ...Args>
		EnableIsBasedOf<BaseComponent, ComponentType>* AddComponent(GameObjectId id, Args&&... args);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType>* AddComponent(ComponentType&& val);

		/// <summary>
		/// Return archetype of the given id
		/// for run-time purposes this should not create new object
		/// </summary>
		/// <param name="archetypeId"></param>
		/// <returns>pointer to existing archetype or nullptr</returns>
		std::pair<bool, WeakPtr<Archetype>> GetArchetype(const std::initializer_list<ComponentTypeId>& archetypeId);
		std::pair<bool, WeakPtr<Archetype>> GetArchetype(const std::vector<ComponentTypeId>& archetypeId);
		std::pair<bool, WeakPtr<Archetype>> GetArchetype(const std::string& combinedArchetypeId);

		void OnStartFrame();
		void OnInputValidation(const SharedPtr<Window>& window);
		void OnPreAsyncUpdate(float dt);
		void OnAsyncUpdate(float dt);
		void OnPhysicsValidation(float dt);
		void OnValidation();
		void OnPreRenderUpdate(float dt);
		void OnRenderSystem(const SharedPtr<Window>& winHandle);
		void SwapCopyBufferToReadBuffer();
		void CleanArchetypeMarked();

		void MarkArchetypeMapDirty();
		void AddGameObjectToWaitingList(GameObjectId id);

		template<typename SystemType,
			typename ...Args>
			EnableIsBasedOf<SystemBase, SystemType, WeakPtr<SystemBase>> AddSystem(Args&&... args);

		template<typename SystemType>
		EnableIsBasedOf<SystemBase, SystemType,WeakPtr<SystemBase>> AddSystem(SystemType&& system);
		
		void DestroyGameObject(GameObjectId id);

		template<typename ComponenType>
		EnableIsBasedOf<BaseComponent, ComponenType> DestroyComponent(GameObjectId id);

		bool IsArchetypeExist(const std::vector<ComponentTypeId>& archetypeId);

		GameObjectId CreateGameObject();

		void RegisterGameObject(const SharedPtr<GameObject>& instance);
		void RegisterGameObject(GameObjectId id);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType> RegisterComponent();

		SharedPtr<BaseComponent> GetRegisterComponent(ComponentTypeId id);

		bool ContainsGameObjectRecord(GameObjectId id);

		size_t GetComponentSizeById(ComponentTypeId id);

		template<typename T>
		EnableIsBasedOf<BaseComponent, T, size_t> GetComponentSizeAndConstruct();

		Record& GetGameObjectRecord(GameObjectId id);

		GameObjectId GetNewId();

		template<typename Func, typename ...Args, typename Ret = std::invoke_result_t<Func, Args...>>
		std::future<Ret> PushAsyncTask(Func&& task, Args&&... args);

		void WakeAllAsyncUpdates();
		void JoinAllAsyncUpdates();

		static std::string ArchetypeIdsToKey(const std::initializer_list<ComponentTypeId>& ids);
		static std::string ArchetypeIdsToKey(const std::vector<ComponentTypeId>& ids);
		static bool IsArchetypeIdsSameAsKey(const std::string& key, const std::string& combinedKey);
		static void RelocateComponentData(int indexInComponent, const SharedPtr<Archetype>& archetype);
		static void RelocateComponentData(WorldEntity& world, int indexInComponent, const SharedPtr<Archetype>& archetype);

		const SharedPtr<b2World>& GetPhysicsWorld() const;
		void AddPhysiscsBodyDef(const PhysicsUserData& data);
		size_t GetOffsetOfComponent(const SharedPtr<Archetype>& archetype, size_t componentIdx);

	private:

		struct TickComponentPool
		{
			TickComponentPool()
				: resource()
				, logger(&resource)
			{
			}

			PoolResource<64> resource{};
			LoggingResource logger;
		};

		struct NonTickComponentPool
		{
			PoolResource<64> resource{};
			LoggingResource logger{ &resource };
		};

		using ComponentResource = PoolResource<1024>;

		InputSettings m_InputSetting;

		// Packed pool of non-tick components
		std::unordered_map<std::string, NonTickComponentPool> m_NonTickComponent;
		// Packed pool of tick components
		std::unordered_map<std::string, TickComponentPool> m_TickComponent;

		void UpdatePipeline(PipelineLayer layer, float deltatime = 0.0f);
		void RenderPipeline(const SharedPtr<Window>& window, PipelineLayer layer);

		using ArchetypeMap = std::unordered_map<std::string, SharedPtr<Archetype>>;

		std::unordered_map<GameObjectId, Record> m_GameObjectToArchetypeMap;
		std::map<ComponentTypeId, ComponentRecord> m_ComponentMap;
		std::array<std::vector<SharedPtr<SystemBase>>, size_t(PipelineLayer::Count)> m_SystemMap;
		std::array<ArchetypeMap, size_t(ObjectBuffer::Count)> m_ArchetypeBuffer;
		std::vector<std::pair<PipelineLayer, SharedPtr<SystemBase>>> m_PendingAddSystem;
		std::vector<std::pair<PipelineLayer, SharedPtr<SystemBase>>> m_PendingDeleteSystem;

		PersistentThreadWorker m_CopyObjectBufferThread;
		size_t m_ReadBufferIndex{};
		size_t m_CopyBufferIndex{};

		// Physics World
		void CreatePhysicsWorld();
		SharedPtr<b2World> m_PhysicsWorld;
		SharedPtr<ContactFilter> m_PhysicsContactFilter;
		std::vector<PhysicsUserData> m_PhysicsRuntimeData;

		// Thread Pool
		SimpleThreadPool m_ThreadPool;

		std::vector<RenderObject> m_RenderObjects;
		
		// To be delete GameObject Id
		std::vector<GameObjectId> m_ToDestroyGameObjects;
		std::vector<GameObjectId> m_ToAddGameObjects;
		std::vector<MoveComponentRecord> m_ToMoveGameObjects;
		
		std::unordered_set<SharedPtr<NonTickComponent>> m_NonTickComponents;

		void DestroyPendingGameObject();
		void ValidateSystems();
		void ValidateArchetype();
		void ValidatePhysicsData();
		
		struct RenderData
		{
			RenderData(size_t startSize)
				: resource(startSize)
				, totalElement()
			{
			}

			UnSynchronizedSpatialResource resource;
			uint32_t totalElement;
		};

		RenderData m_RenderData;

		RenderBuffer m_RenderBuffer;

		// Async Tick Task
		struct TickTask
		{
			std::string id{};
			PersistentThreadWorker worker{};
		};

		std::vector<TickTask> m_TickTasks;

		template<typename SubTickComponent,
			typename = EnableIsBasedOf<TickComponent, SubTickComponent>>
			void InitializeAsyncTickTask(const ObservePtr<SubTickComponent>& pointerToObj);

		template<typename SubTickComponent,
			typename = EnableIsBasedOf<TickComponent, SubTickComponent>>
			void InitializeAsyncTickTask(const SharedPtr<ObservePtr<SubTickComponent>>& component);

		void CopyFromWriteBufferToCopyBuffer();
		PersistentThreadWorker& EmplaceNewWorker(const std::string& id);

		GameObjectId m_GameObjectCounter;
		bool m_ShouldUpdateArchetypeList{};
	};

	template <typename ComponentType, typename, typename ... Args>
	SharedPtr<ObservePtr<ComponentType>> WorldEntity::AllocTickComponent(Args&&... args)
	{
		// This only works if we map of different types of components
		constexpr const char* uniqueId{ ComponentType::UniqueId };

		auto it = m_TickComponent.find(uniqueId);
		if (it == m_TickComponent.end())
			it = m_TickComponent.try_emplace(uniqueId, TickComponentPool{}).first;

		LoggingResource& logResource{ it->second.logger };
		auto& realResource{ it->second.resource };

		// NOTE: I don't know I have to reassign this again to make it works
		logResource = LoggingResource{ &it->second.resource };

		void* allocPtr{ logResource.do_allocate(sizeof(ComponentType),alignof(ComponentType)) };

		auto& bufferHead{ realResource.GetBufferHead() };
		const size_t offSetFromHead{ size_t(std::abs(bufferHead - (uint8_t*)allocPtr)) };

		// Construct
		new (allocPtr) ComponentType(std::forward<Args>(args)...);

		SharedPtr<ObservePtr<ComponentType>> out{ new ObservePtr<ComponentType>{bufferHead,offSetFromHead},[&logResource](ObservePtr<ComponentType>* ptr)
		{
			uint8_t* temp{ptr->GetPtrPointTo()};
			if (ptr->GetPtr())
				ptr->GetPtr()->~ComponentType();
			logResource.do_deallocate(temp,sizeof(ComponentType),alignof(ComponentType));
			delete ptr;
			ptr = nullptr;
		} };

		InitializeAsyncTickTask(out);

		return out;
	}

	template <typename ComponentType, typename, typename ... Args>
	SharedPtr<ObservePtr<ComponentType>> WorldEntity::AllocNonTickComponent(Args&&... args)
	{
		constexpr const char* uniqueId{ ComponentType::UniqueId };

		auto it{ m_NonTickComponent.find(uniqueId) };
		if (it == m_NonTickComponent.end())
			it = m_NonTickComponent.try_emplace(uniqueId, NonTickComponentPool{}).first;
		LoggingResource& logResource{ it->second.logger };

		// NOTE: I don't know I have to reassign this again so it works
		logResource = LoggingResource{ &it->second.resource };
		auto& realResource{ it->second.resource };

		//void* allocPtr{ logResource.do_allocate(sizeof(InputComponent),alignof(InputComponent)) };

		void* allocPtr{ logResource.do_allocate(sizeof(ComponentType),alignof(ComponentType)) };

		// special case for InputComponent
		//if constexpr (std::is_same_v<ComponentType, InputComponent>)
		//{
		//	InputComponent* initPtr{ new (allocPtr) InputComponent{m_InputSetting} };
		//	SharedPtr<InputComponent> out{ initPtr,[&logResource](InputComponent* ptr)
		//	{
		//		logResource.do_deallocate(ptr,sizeof(InputComponent),alignof(InputComponent));
		//		ptr = nullptr;
		//	} };

		//	table.emplace_back(out);

		//	return out;
		//}

		new (allocPtr) ComponentType{ std::forward<Args>(args)... };

		auto& bufferHead{ realResource.GetBufferHead() };
		const size_t offSetFromHead{ size_t(std::abs(bufferHead - (uint8_t*)allocPtr)) };

		ObservePtr<ComponentType>* ob{ new ObservePtr<ComponentType>{bufferHead,offSetFromHead} };

		SharedPtr<ObservePtr<ComponentType>> out{ ob,[this,&logResource](ObservePtr<ComponentType>* ptr)
		{
			uint8_t* temp{ptr->GetPtrPointTo()};
			if (ptr->GetPtr())
				ptr->GetPtr()->~ComponentType();
			logResource.do_deallocate(temp,sizeof(ComponentType),alignof(ComponentType));

			// Remove if this is render elements
			if constexpr (std::is_same_v<Rendering2Component,ComponentType>)
			{
				//m_RenderElements.erase(std::remove_if(m_RenderElements.begin(), m_RenderElements.end(), [ptr](const SharedPtr<ObservePtr<Rendering2Component>>& elem)
				//	{
				//		if (elem.get() == ptr)
				//			return ptr;
				//	}));
			}

			delete ptr;
			ptr = nullptr;
		} };

		//if constexpr (std::is_same_v<Rendering2Component, ComponentType>)
		//	m_RenderElements.emplace_back(out);

		return out;
	}


	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType>* WorldEntity::GetComponent(GameObjectId id)
	{
		const size_t componentTypeId{ ComponentT<ComponentType>::GetId() };

		const Record& rec{ m_GameObjectToArchetypeMap.at(id) };

		if (const auto checkPtr{ rec.restedArchetype.lock() };
			checkPtr)
		{
			size_t strideOffset{};
			for (const auto& compTypeId :  checkPtr->types)
			{
				if (componentTypeId == compTypeId)
					return reinterpret_cast<ComponentType*>(
						&checkPtr->componentData[
							size_t(rec.componentStructIdx * checkPtr->sizeOfStruct) +
								strideOffset]);

				strideOffset += m_ComponentMap.at(compTypeId).size;
			}
		}

		return nullptr;
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType>* WorldEntity::AddComponent(ComponentType&& val)
	{
		return nullptr;
	}

	template <typename SystemType, typename ... Args>
	EnableIsBasedOf<SystemBase, SystemType, WeakPtr<SystemBase>> WorldEntity::AddSystem(Args&&... args)
	{
		SharedPtr<SystemBase> base{ new SystemType(std::forward<Args>(args)...) };

		if constexpr (std::is_base_of_v<b2ContactListener, SystemType>)
			m_PhysicsWorld->SetContactListener(dynamic_cast<b2ContactListener*>(base.get()));
		
		m_PendingAddSystem.emplace_back(std::make_pair(base->GetLayer(), base));
		
		return base;
	}

	template <typename SystemType>
	EnableIsBasedOf<SystemBase, SystemType, WeakPtr<SystemBase>> WorldEntity::AddSystem(SystemType&& system)
	{
		SharedPtr<SystemBase> base{ new SystemType(std::forward<SystemType>(system)) };

		if constexpr (std::is_base_of_v<b2ContactListener, SystemType>)
			m_PhysicsWorld->SetContactListener(dynamic_cast<b2ContactListener*>(base.get()));
		
		m_PendingAddSystem.emplace_back(std::make_pair(base->GetLayer(), base));
		return base;
	}

	template <typename ComponenType>
	EnableIsBasedOf<BaseComponent, ComponenType> WorldEntity::DestroyComponent(GameObjectId id)
	{
		Record& rec{ m_GameObjectToArchetypeMap.at(id) };
		if (const auto archetype{ rec.restedArchetype.lock() };
			archetype)
		{
			
			const size_t targetCompTypeId{ ComponentT<ComponenType>::GetId() };

			const auto findIter{ std::find(archetype->types.begin(),archetype->types.end(),targetCompTypeId) };

			if (findIter == archetype->types.end())
				return;
			
			std::vector<size_t> newTypes{ archetype->types };
			newTypes.erase(findIter);

			MoveComponentRecord moveRec{};
			moveRec.id = id;
			moveRec.oldArchetype = archetype;
			moveRec.oldCompIndex = rec.componentStructIdx;
			moveRec.componentId = targetCompTypeId;

			m_ToMoveGameObjects.emplace_back(moveRec);

			const auto& [isExist, newArchetype] = GetArchetype(newTypes);

			SharedPtr<Archetype> destinedArchetype = newArchetype.lock();

			if(!isExist)
			{
				destinedArchetype->types = newTypes;
				
				// register size of components
				for (const auto& typeId : destinedArchetype->types)
				{
					destinedArchetype->sizeOfStruct += int(m_ComponentMap.at(typeId).size);
				}
			}
			
						
			AddGameObjectToWaitingList(id);

			rec.restedArchetype = destinedArchetype;
			rec.componentStructIdx = int(destinedArchetype->gameObjectIds.size());
			
			
			//const std::string newCombinedType{ ArchetypeIdsToKey(newTypes) };
			//
			//const ComponentRecord& targetCompRec{ m_ComponentMap.at(targetCompTypeId) };

			//uint8_t* componentAddress{ &archetype->componentData[
			//	rec.componentStructIdx * archetype->sizeOfStruct] };

			//size_t offset{};
			//for (const auto& typeId : archetype->types)
			//{
			//	if (typeId == targetCompTypeId)
			//		break;

			//	offset += m_ComponentMap.at(typeId).size;
			//}

			//// some component can't destroy during iteration
			//targetCompRec.instance->DestroyData(componentAddress + offset);

			//// move this GameObject outside of the old archetype
			//// 1. find if there's existing archetype
			//const auto& writeBuffer{ m_ArchetypeBuffer[ObjectBuffer::Write] };

			//SharedPtr<Archetype> destinedArchetype{};
			//
			//// TODO: Finish this
			//const auto& [isExist,newArchetype] = GetArchetype(newCombinedType);

			//destinedArchetype = newArchetype.lock();

		}
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType> WorldEntity::RegisterComponent()
	{
		const size_t typeId{ ComponentT<ComponentType>::GetId() };

		if (m_ComponentMap.find(typeId) != m_ComponentMap.end())
			return;

		SharedPtr<BaseComponent> component{ std::make_shared<ComponentT<ComponentType>>() };
		const size_t componentSize{ sizeof(ComponentType) };

		m_ComponentMap.emplace_hint(m_ComponentMap.end(), std::make_pair(typeId, ComponentRecord{ component,componentSize }));
	}

	template <typename T>
	EnableIsBasedOf<BaseComponent, T, size_t> WorldEntity::GetComponentSizeAndConstruct()
	{
		// Register first if not done
		RegisterComponent<T>();

		const size_t typeId{ ComponentT<T>::GetId() };
		return m_ComponentMap[typeId].size;
	}

	template <typename Func, typename ... Args, typename Ret>
	std::future<Ret> WorldEntity::PushAsyncTask(Func&& task, Args&&... args)
	{
		return m_ThreadPool.PushTask(std::forward<Func>(task), std::forward<Args>(args)...);
	}


	template <typename SubTickComponent, typename>
	void WorldEntity::InitializeAsyncTickTask(const ObservePtr<SubTickComponent>& pointerToObj)
	{
		// Check if this component's worker is already initialize
		auto fIt = std::find_if(m_TickTasks.begin(), m_TickTasks.end(), [](const TickTask& item)
			{
				return std::strcmp(item.id.c_str(), SubTickComponent::UniqueId) == 0;
			});

		const bool found{ fIt != m_TickTasks.end() };
		PersistentThreadWorker& thread{ found ? fIt->worker : EmplaceNewWorker(SubTickComponent::UniqueId) };
		thread.AssignTask([&pointerToObj, this]()
			{
				pointerToObj.GetPtr()->AsyncUpdate(this->GetDeltaTime());
			});
	}

	template <typename SubTickComponent, typename>
	void WorldEntity::InitializeAsyncTickTask(const SharedPtr<ObservePtr<SubTickComponent>>& component)
	{
		// Check if this component's worker is already initialize
		auto fIt = std::find_if(m_TickTasks.begin(), m_TickTasks.end(), [](const TickTask& item)
			{
				return std::strcmp(item.id.c_str(), SubTickComponent::UniqueId) == 0;
			});

		const bool found{ fIt != m_TickTasks.end() };
		PersistentThreadWorker& thread{ found ? fIt->worker : EmplaceNewWorker(SubTickComponent::UniqueId) };
		WeakPtr<ObservePtr<SubTickComponent>> weak{ component };
		thread.AssignTask([weak, this]()
			{
				if (auto real = weak.lock();
					real)
				{
					real->GetPtr()->AsyncUpdate(this->GetDeltaTime());
				}
				//pointerToObj.GetPtr()->AsyncUpdate(this->GetDeltaTime());
			});
	}
}

#endif