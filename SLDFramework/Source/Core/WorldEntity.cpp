#include "WorldEntity.h"
#include "../Components/RenderingComponent.h"
#include "../GameObject/GameObject.h"

SLD::WorldEntity::WorldEntity()
	: m_TickComponent()
	, m_RenderComponents()
	, m_TickTasks()
	, m_EndTimePoint()
	, m_CurrentTimePoint()
	, m_DeltaTime()
{
	m_TickTasks.reserve(size_t(TickComponent::Type::Count));
}


RefPtr<SLD::RenderingComponent> SLD::WorldEntity::AllocRenderComponent(size_t elemSize, uint32_t elemCnt)
{
	auto& component{ m_RenderComponents.emplace_back(elemSize,elemCnt) };
	RefPtr<RenderingComponent> smart{ &component,[](RenderingComponent* component)
	{
		component = nullptr;
	} };
	return smart;
}

SLD::RenderingComponent& SLD::WorldEntity::AllocRefRenderComponent(size_t elemSize, uint32_t elemCnt)
{
	return m_RenderComponents.emplace_back(elemSize, elemCnt);
}

const std::vector<SLD::RenderingComponent>& SLD::WorldEntity::GetAllRenderComponents() const
{
	return m_RenderComponents;
}

std::vector<SLD::RenderingComponent>& SLD::WorldEntity::GetAllRenderingComponentsEditable()
{
	return m_RenderComponents;
}

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
	if (m_IsFirstFrame)
	{
		m_EndTimePoint = m_CurrentTimePoint;
		m_IsFirstFrame = false;
	}

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


