#include "WorldEntity.h"
#include "../Components/RenderingComponent.h"
#include "../GameObject/GameObject.h"

SLD::WorldEntity::WorldEntity()
	: m_TickComponent()
	//, m_RenderComponents()
	, m_RenderData(1024)
	, m_TickTasks()
	, m_EndTimePoint()
	, m_CurrentTimePoint()
	, m_DeltaTime()
{
	m_TickTasks.reserve(size_t(TickComponent::Type::Count));
}

RefPtr<SLD::ObservePtr<SLD::RenderingComponent>> SLD::WorldEntity::AllocRenderingComponent(
	const RefPtr<ObservePtr<TransformComponent>>& transform, size_t elemSize, uint32_t elemCnt)
{
	constexpr const char* uniqueId{ RenderingComponent::UniqueId };
	auto it{ m_NonTickComponent.try_emplace(uniqueId,NonTickComponentPool{}) };
	LoggingResource& logResource{ it.first->second.logger };

	// NOTE: I don't know I have to reassign this again so it works
	logResource = LoggingResource{ &it.first->second.resource };
	auto& realResource{ it.first->second.resource };

	void* allocPtr{ logResource.do_allocate(sizeof(RenderingComponent),alignof(RenderingComponent)) };

	// Handle Render Buffer
	RefPtr<uint8_t> pointerToBuffer{};
	{
		void* renderPointer{ m_RenderData.resource.do_allocate(elemSize, alignof(std::max_align_t)) };
		uint8_t* castPointer{ static_cast<uint8_t*>(renderPointer) };
		pointerToBuffer = RefPtr<uint8_t>{ castPointer, [this, elemSize](uint8_t* ptr)
		{
			uint8_t* temp{};
			std::memcpy(&temp, ptr, sizeof(void*));
			m_RenderData.resource.do_deallocate(temp, elemSize, alignof(std::max_align_t));
		} };


		uint8_t* temp{};
		std::memcpy(&temp, castPointer, sizeof(void*));
		const size_t offset{ size_t(temp - m_RenderData.resource.GetHead()) };
		const ObservePtr<std::nullptr_t> bufferObserver{ m_RenderData.resource.GetHead(),offset };
		
		// Construct
		new (allocPtr) RenderingComponent{ transform,pointerToBuffer,bufferObserver,elemSize,elemCnt };
	}
	

	
	auto& bufferHead{ realResource.GetBufferHead() };
	const size_t offSetFromHead{ size_t(std::abs(bufferHead - (uint8_t*)allocPtr)) };

	ObservePtr<RenderingComponent>* ob{ new ObservePtr<RenderingComponent>{bufferHead,offSetFromHead} };

	RefPtr<ObservePtr<RenderingComponent>> out{ ob,[this,&logResource](ObservePtr<RenderingComponent>* ptr)
	{
		//ptr->GetPtr()->~RenderingComponent();
		logResource.do_deallocate(ptr->GetPtr(),sizeof(RenderingComponent),alignof(RenderingComponent));
		m_RenderData.totalElement--;
		delete ptr;
		ptr = nullptr;
	} };
	
	m_RenderData.totalElement++;

	return out;
}

//RefPtr<SLD::RenderingComponent> SLD::WorldEntity::AllocRenderComponent(const RefPtr<ObservePtr<TransformComponent>>& transform, size_t elemSize,
//                                                                       uint32_t elemCnt)
//{
//	auto& buffer{ m_RenderingPoolComponent.buffer };
//	auto& head{ m_RenderingPoolComponent.head };
//
//	const size_t offset{ buffer.size() };
//	buffer.resize(elemSize);
//	head = buffer.data();
//
//	RenderingComponent component{ transform,head,offset,elemSize,elemCnt };
//	auto& emplace{ m_RenderComponents.emplace_back(component) };
//	
//	return RefPtr<RenderingComponent>{&emplace, [](RenderingComponent* ptr)
//		{
//			
//			ptr = nullptr;
//		}};
//}

uint8_t* SLD::WorldEntity::GetRenderDataBufferStart() const noexcept
{
	return m_RenderData.resource.GetHead();
}

uint32_t SLD::WorldEntity::GetRenderElementCount() const noexcept
{
	return m_RenderData.totalElement;
}

size_t SLD::WorldEntity::GetRenderDataSize() const noexcept
{
	return m_RenderData.resource.GetUsedSize();
}


//RefPtr<SLD::RenderingComponent> SLD::WorldEntity::AllocRenderComponent(size_t elemSize, uint32_t elemCnt)
//{
//	auto& component{ m_RenderComponents.emplace_back(elemSize,elemCnt) };
//	RefPtr<RenderingComponent> smart{ &component,[](RenderingComponent* component)
//	{
//		component = nullptr;
//	} };
//	return smart;
//}
//
//SLD::RenderingComponent& SLD::WorldEntity::AllocRefRenderComponent(size_t elemSize, uint32_t elemCnt)
//{
//	return m_RenderComponents.emplace_back(elemSize, elemCnt);
//}
//
//const std::vector<SLD::RenderingComponent>& SLD::WorldEntity::GetAllRenderComponents() const
//{
//	return m_RenderComponents;
//}
//
//std::vector<SLD::RenderingComponent>& SLD::WorldEntity::GetAllRenderingComponents()
//{
//	return m_RenderComponents;
//}

SLD::InputSetting& SLD::WorldEntity::GetWorldInputSetting()
{
	return m_InputSetting;
}

RefPtr<SLD::GameObject> SLD::WorldEntity::CreateGameObject()
{
	return std::make_shared<GameObject>(*this);
}

float SLD::WorldEntity::GetDeltaTime() const noexcept
{
	return m_DeltaTime;
}

void SLD::WorldEntity::Destroy(const RefPtr<GameObject>& object)
{
	object;
}

void SLD::WorldEntity::Destroy(const GameObject& object)
{
	object;
}

void SLD::WorldEntity::WakeAllAsyncUpdates()
{
	for (auto& task : m_TickTasks)
	{
		task.worker.Wake();
	}
}

void SLD::WorldEntity::JoinAllAsyncUpdates()
{
	for (auto& task : m_TickTasks)
	{
		task.worker.WaitTillFinished();
	}
}

void SLD::WorldEntity::StartWorldTime()
{
	m_CurrentTimePoint = std::chrono::high_resolution_clock::now();
	//if (m_IsFirstFrame)
	//{
	//	m_EndTimePoint = m_CurrentTimePoint;
	//	m_IsFirstFrame = false;
	//}

	// https://stackoverflow.com/questions/34141522/c-incorrect-fps-and-deltatime-measuring-using-stdchrono
	using ms = std::chrono::duration<float, std::milli>;
	m_DeltaTime = std::chrono::duration_cast<ms>(m_CurrentTimePoint - m_EndTimePoint).count() * 0.001f;
	//m_DeltaTime = 1.0f / 60.0f;
}

void SLD::WorldEntity::EndWorldTime()
{
	m_EndTimePoint = m_CurrentTimePoint;
}

SLD::PersistentThreadWorker& SLD::WorldEntity::EmplaceNewWorker(const std::string& id)
{
	auto& thread{ m_TickTasks.emplace_back(TickTask{ id,{} }).worker };
	thread.Start();
	return thread;
}


