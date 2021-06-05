#ifndef SLDFRAMEWORK_WORLDENTITY_H
#define SLDFRAMEWORK_WORLDENTITY_H

#include "Base.h"
#include "PMR_Resource/LoggingResource.h"
#include "PMR_Resource/PoolResource.h"
#include "../Inputs/InputSetting.h"
#include "../Components/TickComponent.h"
#include "PersistentThreadWorker.h"
#include "PMR_Resource/UnSynchronizedSpatialResource.h"

namespace SLD
{
	class RenderingComponent;
	class NonTickComponent;
	class TransformComponent;
	class GameObject;
	class WorldEntity final
	{
	public:

		WorldEntity();
		~WorldEntity();

		// Won't return anything because vector loses reference when moved
		//RefPtr<RenderingComponent> AllocRenderComponent(const RefPtr<ObservePtr<TransformComponent>>& transform, size_t elemSize, uint32_t elemCnt);

		RefPtr<ObservePtr<RenderingComponent>> AllocRenderingComponent(
			const RefPtr<ObservePtr<TransformComponent>>& transform,
			size_t elemSize,
			uint32_t elemCnt);

		uint8_t* GetRenderDataBufferStart() const noexcept;
		uint32_t GetRenderElementCount() const noexcept;
		size_t GetRenderDataSize() const noexcept;
		
		template<typename ComponentType,
			typename = EnableIsBasedOf<TickComponent, ComponentType >,
			typename ...Args>
			RefPtr<ObservePtr<ComponentType>> AllocTickComponent(Args&&... args);
		
		template<typename ComponentType,
			typename = EnableIsBasedOf<NonTickComponent, ComponentType>,
			typename ...Args>
			RefPtr<ObservePtr<ComponentType>> AllocNonTickComponent(Args&&... args);
		
		template<typename ComponentType,
			typename = EnableIsBasedOf<TickComponent, ComponentType>>
			void DeAllocTickComponent(const RefPtr<ComponentType>& ptr);

		[[nodiscard]] const std::vector<RenderingComponent>& GetAllRenderComponents() const;
		[[nodiscard]] std::vector<RenderingComponent>& GetAllRenderingComponents();

		[[nodiscard]] InputSetting& GetWorldInputSetting();

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

		InputSetting m_InputSetting;

		// Packed pool of non-tick components
		std::unordered_map<std::string, NonTickComponentPool> m_NonTickComponent;
		// Packed pool of tick components
		std::unordered_map<std::string, TickComponentPool> m_TickComponent;

		// TODO: This is really exposes maybe encapsulate later
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

		PersistentThreadWorker& EmplaceNewWorker(const std::string& id);

		// Game Related
		std::chrono::high_resolution_clock::time_point m_EndTimePoint;
		std::chrono::high_resolution_clock::time_point m_CurrentTimePoint;
		float m_DeltaTime;
		bool m_IsFirstFrame{ true };
	};

	template <typename ComponentType, typename, typename ... Args>
	RefPtr<ObservePtr<ComponentType>> WorldEntity::AllocTickComponent(Args&&... args)
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

		new (allocPtr) ComponentType(std::forward<Args>(args)...);

		RefPtr<ObservePtr<ComponentType>> out{ new ObservePtr<ComponentType>{bufferHead,offSetFromHead},[&logResource](ObservePtr<ComponentType>* ptr)
		{
			uint8_t* temp{ptr->GetPtrPointTo()};
			if (ptr->GetPtr())
				ptr->GetPtr()->~ComponentType();
			logResource.do_deallocate(temp,sizeof(ComponentType),alignof(ComponentType));
			delete ptr;
			ptr = nullptr;
		} };

		InitializeAsyncTickTask(*out.get());

		return out;
	}
	
	template <typename ComponentType, typename, typename ... Args>
	RefPtr<ObservePtr<ComponentType>> WorldEntity::AllocNonTickComponent(Args&&... args)
	{
		constexpr const char* uniqueId{ ComponentType::UniqueId };

		auto it{ m_NonTickComponent.find(uniqueId) };
		if(it == m_NonTickComponent.end())
			it = m_NonTickComponent.try_emplace(uniqueId,NonTickComponentPool{}).first;
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
		//	RefPtr<InputComponent> out{ initPtr,[&logResource](InputComponent* ptr)
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

		RefPtr<ObservePtr<ComponentType>> out{ ob,[&logResource](ObservePtr<ComponentType>* ptr)
		{
			uint8_t* temp{ptr->GetPtrPointTo()};
			if (ptr->GetPtr())
				ptr->GetPtr()->~ComponentType();
			logResource.do_deallocate(temp,sizeof(ComponentType),alignof(ComponentType));
			delete ptr;
			ptr = nullptr;
		} };


		return out;
	}



	template <typename ComponentType, typename>
	void WorldEntity::DeAllocTickComponent(const RefPtr<ComponentType>& ptr)
	{
		constexpr std::string uniqueId{};
		auto& tickPool{ m_TickComponent.at(uniqueId) };

		tickPool;
	}

	//template <typename SubTickComponent, typename>
	//void WorldEntity::InitializeAsyncTickTask(SubTickComponent& object)
	//{
	//	// Check if this component's worker is already initialize
	//	auto fIt = std::find_if(m_TickTasks.begin(), m_TickTasks.end(), [](const TickTask& item)
	//		{
	//			return std::strcmp(item.id.c_str(), SubTickComponent::UniqueId) == 0;
	//		});

	//	const bool found{ fIt != m_TickTasks.end() };
	//	PersistentThreadWorker& thread{ found ? fIt->worker : EmplaceNewWorker(SubTickComponent::UniqueId) };
	//	thread.AssignTask(&SubTickComponent::AsyncUpdate, object, this->GetDeltaTime());
	//}

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
}

#endif