#include "GameObject.h"

SLD::GameObject::GameObject(WorldEntity& world)
	: m_World(world)
	, m_Transform(world.AllocTickComponent<TransformComponent>())
{
}

const RefPtr<SLD::ObservePtr<SLD::TransformComponent>>& SLD::GameObject::GetTransform() const
{
	return m_Transform;
}

//const std::vector<RefPtr<SLD::BaseComponent>>& SLD::GameObject::GetAllComponents() const
//{
//	return m_ComponentTable;
//}

RefPtr<SLD::RenderingComponent> SLD::GameObject::CreateRenderingComponent(size_t elemSize, uint32_t elemCnt)
{
	return m_World.get().AllocRenderComponent(elemSize, elemCnt);
}

RefWrap<SLD::WorldEntity> SLD::GameObject::GetWorld()
{
	return m_World;
}

SLD::GameObject::GameObject(const GameObject& other)
	: m_ComponentTable(other.m_ComponentTable)
	, m_World(other.m_World)
	, m_Transform(other.m_Transform)
{

}

SLD::GameObject& SLD::GameObject::operator=(const GameObject& other)
{
	if (this != &other)
	{
		m_World = other.m_World;
		m_Transform = other.m_Transform;
		m_ComponentTable = other.m_ComponentTable;
	}

	return *this;
}

SLD::GameObject::GameObject(GameObject&& other) noexcept
	: m_ComponentTable(std::move(other.m_ComponentTable))
	, m_World(std::move(other.m_World))
	, m_Transform(std::move(other.m_Transform))
{
}

SLD::GameObject& SLD::GameObject::operator=(GameObject&& other) noexcept
{
	if(this != &other)
	{
		m_World = std::move(other.m_World);
		m_Transform = std::move(other.m_Transform);
		m_ComponentTable = std::move(other.m_ComponentTable);
	}

	return *this;
}

SLD::GameObject::~GameObject()
{
	m_Transform.reset();
}
