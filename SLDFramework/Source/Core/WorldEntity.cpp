#include "WorldEntity.h"
#include "../Components/RenderingComponent.h"
#include "../GameObject/GameObject.h"

SLD::WorldEntity::WorldEntity()
	: m_TickComponent()
	, m_RenderComponents()
	, m_EndTimePoint()
	, m_DeltaTime()
{
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

const SLD::InputSetting& SLD::WorldEntity::GetWorldInputSetting() const
{
	return m_InputSetting;
}

RefPtr<SLD::GameObject> SLD::WorldEntity::CreateGameObject()
{	
	return std::make_shared<GameObject>(shared_from_this());
}

float SLD::WorldEntity::GetDeltaTime()
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

void SLD::WorldEntity::StartWorldTime()
{
	auto startTime{ std::chrono::system_clock::now()  };
	m_DeltaTime = float(std::chrono::duration_cast<std::chrono::milliseconds>(startTime - m_EndTimePoint).count());
}

void SLD::WorldEntity::EndWorldTime()
{
	m_EndTimePoint = std::chrono::system_clock::now();
}
