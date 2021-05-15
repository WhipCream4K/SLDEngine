#include "GameObject.h"

SLD::GameObject::GameObject(const RefPtr<WorldEntity>& world)
	: m_World(world)
	, m_Transform()
{
	m_Transform = world->AllocTickComponent<TransformComponent>();
}

SLD::GameObject::GameObject(WorldEntity& world)
{
	m_World = RefPtr<WorldEntity>{ &world,[](WorldEntity*) {} };
	m_Transform = m_World->AllocTickComponent<TransformComponent>();
}

RefPtr<SLD::TransformComponent> SLD::GameObject::GetTransform() const
{
	return m_Transform.lock();
}

const std::vector<RefPtr<SLD::BaseComponent>>& SLD::GameObject::GetAllComponents() const
{
	return m_ComponentTable;
}

RefPtr<SLD::RenderingComponent> SLD::GameObject::CreateRenderingComponent(size_t elemSize, uint32_t elemCnt)
{
	//auto world{ m_World.lock() };
	return m_World->AllocRenderComponent(elemSize, elemCnt);
}
