#include "GameObject.h"
//#include "../Components/TransformComponent.h"

//SLD::GameObject::GameObject(const RefPtr<WorldEntity>& world)
//	: m_World(world)
//	, m_Transform()
//{
//	m_Transform = world->AllocTickComponent<TransformComponent>();
//}

SLD::GameObject::GameObject(WorldEntity& world)
	: m_World(world)
	, m_Transform(world.AllocTickComponent<TransformComponent>())
{
}

const SLD::ObservePtr<SLD::TransformComponent>& SLD::GameObject::GetTransform() const
{
	return m_Transform;
}

const std::vector<RefPtr<SLD::BaseComponent>>& SLD::GameObject::GetAllComponents() const
{
	return m_ComponentTable;
}

RefPtr<SLD::RenderingComponent> SLD::GameObject::CreateRenderingComponent(size_t elemSize, uint32_t elemCnt)
{
	return m_World.get().AllocRenderComponent(elemSize, elemCnt);
}

RefWrap<SLD::WorldEntity> SLD::GameObject::GetWorld()
{
	return m_World;
}
