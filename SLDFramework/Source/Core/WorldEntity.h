#ifndef SLDFRAMEWORK_WORLDENTITY_H
#define SLDFRAMEWORK_WORLDENTITY_H

#include "Base.h"
#include "ObservePtr.h"
#include "PMR_Resource/LoggingResource.h"
#include "PMR_Resource/PoolResource.h"
#include "../Inputs/InputSetting.h"
#include "../Components/InputComponent.h"
#include "../Components/TickComponent.h"
#include "../Core/PersistentThreadWorker.h"

namespace SLD
{
	class RenderingComponent;
	class TickComponent;
	class NonTickComponent;
	class GameObject;
	class WorldEntity final
	{
	public:

		WorldEntity();

		RefPtr<RenderingComponent> AllocRenderComponent(size_t elemSize, uint32_t elemCnt);
		RenderingComponent& AllocRefRenderComponent(size_t elemSize, uint32_t elemCnt);

		//template<typename ComponentType,
		//	typename = EnableIsBasedOf<TickComponent,ComponentType>,
		//	typename ...Args>
		//	RefPtr<ComponentType> AllocTickComponent(Args&&... args);

		//template<typename ComponentType,
		//	typename = EnableIsBasedOf<TickComponent, ComponentType>,
		//	typename ...Args>
		//	OwnedPtr<ComponentType, No_Op<ComponentType>> AllocTickComponent(Args&&... args);

		// TODO: Can't find a smart pointer that takes no ownership
		// until I can find one I will return raw pointers :(
		template<typename ComponentType,
			typename = EnableIsBasedOf<TickComponent, ComponentType>,
			typename ...Args>
			ObservePtr<ComponentType> AllocTickComponent(Args&&... args);

		template<typename ComponentType,
			typename = EnableIsBasedOf<NonTickComponent, ComponentType>,
			typename ...Args>
			RefPtr<ComponentType> AllocNonTickComponent(Args&&... args);

		template<typename ComponentType,
			typename = EnableIsBasedOf<TickComponent, ComponentType>>
			void DeAllocTickComponent(const RefPtr<ComponentType>& ptr);

		[[nodiscard]] const std::vector<RenderingComponent>& GetAllRenderComponents() const;
		[[nodiscard]] std::vector<RenderingComponent>& GetAllRenderingComponentsEditable();

		[[nodiscard]] const InputSetting& GetWorldInputSetting() const;

		[[nodiscard]] RefPtr<GameObject> CreateGameObject();

		[[nodiscard]] float GetDeltaTime() const noexcept;

		// Detach all of the component from this GameObject
		void Destroy(const RefPtr<GameObject>& object);
		// Detach all of the component from this GameObject
		void Destroy(const GameObject& object);

		void WakeAllAsyncUpdates();
		void JoinAllAsyncUpdates();

		void StartWorldTime();
		void EndWorldTime();

	private:

		struct TickComponentPool
		{
			TickComponentPool()
				: resource()
				, logger(&resource)
				, dataTable()
			{
			}

			PoolResource<64> resource{};
			LoggingResource logger;
			std::vector<RefPtr<ObservePtr<TickComponent>>> dataTable{};
		};

		struct NonTickComponentPool
		{
			PoolResource<64> resource{};
			LoggingResource logger{ &resource };
			std::vector<RefPtr<NonTickComponent>> dataTable{};
		};

		InputSetting m_InputSetting;

		// Packed pool of non-tick components
		std::unordered_map<std::string, NonTickComponentPool> m_NonTickComponent;
		// Packed pool of tick components
		std::unordered_map<std::string, TickComponentPool> m_TickComponent;

		// Uncouple from the base component above
		std::vector<RenderingComponent> m_RenderComponents;

		// Async Tick Task
		struct TickTask
		{
			std::string id{};
			PersistentThreadWorker worker{};
		};

		std::vector<TickTask> m_TickTasks;

		template<typename SubTickComponent,
			typename = EnableIsBasedOf<TickComponent, SubTickComponent>>
			void InitializeAsyncTickTask(SubTickComponent& object);

		template<typename SubTickComponent,
			typename = EnableIsBasedOf<TickComponent, SubTickComponent>>
			void InitializeAsyncTickTask(SubTickComponent** pointerToObj);

		PersistentThreadWorker& EmplaceNewWorker(const std::string& id);

		// Game Related
		std::chrono::high_resolution_clock::time_point m_EndTimePoint;
		float m_DeltaTime;
	};

	//template <typename ComponentType, typename, typename ... Args>
	//RefPtr<ComponentType> WorldEntity::AllocTickComponent(Args&&... args)
	//{
	//	// This only works if we map of different types of components
	//	constexpr const char* uniqueId{ ComponentType::UniqueId };

	//	auto it{ m_TickComponent.try_emplace(uniqueId, TickComponentPool{}) };

	//	auto& logResource{ it.first->second.logger };

	//	// NOTE: I don't know I have to reassign this again so it works
	//	logResource = LoggingResource{ &it.first->second.resource };

	//	auto& table{ it.first->second.dataTable };

	//	void* allocPtr{ logResource.do_allocate(sizeof(ComponentType),alignof(ComponentType)) };

	//	ComponentType* initPtr{ new (allocPtr) ComponentType(std::forward<Args>(args)...) };

	//	//RefWrap<PointTo<ComponentType>> test{&initPtr};
	//	
	//	//RefPtr<ComponentType> out{ initPtr,[&logResource](ComponentType* ptr)
	//	//{
	//	//	logResource.do_deallocate(ptr,sizeof(ComponentType),alignof(ComponentType));
	//	//	ptr = nullptr;
	//	//} };

	//	
	//	
	//	table.emplace_back();

	//	// Assign Tick Task
	//	InitializeAsyncTickTask(*out);

	//	return out;

	//}

	template <typename ComponentType, typename, typename ... Args>
	ObservePtr<ComponentType> WorldEntity::AllocTickComponent(Args&&... args)
	{
		// This only works if we map of different types of components
		constexpr const char* uniqueId{ ComponentType::UniqueId };

		auto it{ m_TickComponent.try_emplace(uniqueId, TickComponentPool{}) };

		auto& logResource{ it.first->second.logger };

		// NOTE: I don't know I have to reassign this again so it works
		logResource = LoggingResource{ &it.first->second.resource };

		auto& table{ it.first->second.dataTable };

		void* allocPtr{ logResource.do_allocate(sizeof(ComponentType),alignof(ComponentType)) };

		ComponentType* initPtr{ new (allocPtr) ComponentType(std::forward<Args>(args)...) };

		ObservePtr<TickComponent>* observeBase{ new ObservePtr<TickComponent>(initPtr) };
		auto smart{ std::make_shared<ObservePtr<TickComponent>>() };
		smart.reset(observeBase, [](ObservePtr<TickComponent>* ptr)
			{
				delete ptr;
				ptr = nullptr;
			});
	
		
		//RefPtr<ObservePtr<TickComponent>> addressValue{ &observeBase,[&logResource](ObservePtr<TickComponent>* ptr)
		//{
		//	logResource.do_deallocate(*(*ptr),sizeof(ComponentType),alignof(ComponentType));
		//	ptr = nullptr;
		//} };

		auto& outValue = table.emplace_back(smart);

		InitializeAsyncTickTask(&initPtr);
		
		return ObservePtr<ComponentType>{static_cast<ComponentType*>(outValue->GetPtr())};
	}

	template <typename ComponentType, typename, typename ... Args>
	RefPtr<ComponentType> WorldEntity::AllocNonTickComponent(Args&&... args)
	{
		constexpr const char* uniqueId{ ComponentType::UniqueId };
		auto it{ m_NonTickComponent.try_emplace(uniqueId,NonTickComponentPool{}) };
		auto& logResource{ it.first->second.logger };

		// NOTE: I don't know I have to reassign this again so it works
		logResource = LoggingResource{ &it.first->second.resource };

		auto& table{ it.first->second.dataTable };
		void* allocPtr{ logResource.do_allocate(sizeof(InputComponent),alignof(InputComponent)) };

		// special case for InputComponent
		if constexpr (std::is_same_v<ComponentType, InputComponent>)
		{
			InputComponent* initPtr{ new (allocPtr) InputComponent{m_InputSetting} };
			RefPtr<InputComponent> out{ initPtr,[&logResource](InputComponent* ptr)
			{
				logResource.do_deallocate(ptr,sizeof(InputComponent),alignof(InputComponent));
				ptr = nullptr;
			} };

			table.emplace_back(out);

			return out;
		}

		ComponentType* initPtr{ new (allocPtr) ComponentType(std::forward<Args>(args)...) };

		RefPtr<ComponentType> out{ initPtr,[&logResource](ComponentType* ptr)
		{
			logResource.do_deallocate(ptr,sizeof(ComponentType),alignof(ComponentType));
			ptr = nullptr;
		} };

		table.emplace_back(out);

		return out;
	}



	template <typename ComponentType, typename>
	void WorldEntity::DeAllocTickComponent(const RefPtr<ComponentType>& ptr)
	{
		constexpr std::string uniqueId{};
		auto& tickPool{ m_TickComponent.at(uniqueId) };
		auto& table{ tickPool.dataTable };

		table.erase(std::remove(table.begin(), table.end(), ptr));
	}

	template <typename SubTickComponent, typename>
	void WorldEntity::InitializeAsyncTickTask(SubTickComponent& object)
	{
		// Check if this component's worker is already initialize
		auto fIt = std::find_if(m_TickTasks.begin(), m_TickTasks.end(), [](const TickTask& item)
			{
				return std::strcmp(item.id.c_str(), SubTickComponent::UniqueId) == 0;
			});

		const bool found{ fIt != m_TickTasks.end() };
		PersistentThreadWorker& thread{ found ? fIt->worker : EmplaceNewWorker(SubTickComponent::UniqueId) };
		thread.AssignTask(&SubTickComponent::AsyncUpdate, object, this->GetDeltaTime());
	}

	template <typename SubTickComponent, typename>
	void WorldEntity::InitializeAsyncTickTask(SubTickComponent** pointerToObj)
	{
		// Check if this component's worker is already initialize
		auto fIt = std::find_if(m_TickTasks.begin(), m_TickTasks.end(), [](const TickTask& item)
			{
				return std::strcmp(item.id.c_str(), SubTickComponent::UniqueId) == 0;
			});

		const bool found{ fIt != m_TickTasks.end() };
		PersistentThreadWorker& thread{ found ? fIt->worker : EmplaceNewWorker(SubTickComponent::UniqueId) };
		thread.AssignTask([pointerToObj,this]()
		{
				(*pointerToObj)->AsyncUpdate(this->GetDeltaTime());
		});
	}
}

#endif