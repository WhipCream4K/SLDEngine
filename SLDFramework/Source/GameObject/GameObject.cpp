#include "GameObject.h"

SLD::GameObject::GameObject(const RefPtr<WorldEntity>& world)
	: m_World(world)
{
}

RefPtr<SLD::TransformComponent> SLD::GameObject::GetTransform()
{
	return m_Transform.lock();
}

const std::vector<RefPtr<SLD::BaseComponent>>& SLD::GameObject::GetAllComponents() const
{
	return m_ComponentTable;
}

RefPtr<SLD::RenderingComponent> SLD::GameObject::CreateRenderingComponent(size_t elemSize, uint32_t elemCnt)
{
	auto world{ m_World.lock() };
	return world->AllocRenderComponent(elemSize, elemCnt);
}
