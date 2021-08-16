#include "GameObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderingComponent.h"

SLD::GameObject::GameObject(WorldEntity& world)
	: m_Id(world.GetNewId())
	, m_World(world)
{
	m_World.RegisterGameObject(m_Id);
	AddComponent<TransformComponent>();
}

const SharedPtr<SLD::ObservePtr<SLD::TransformComponent>>& SLD::GameObject::GetTransform() const
{
	return m_Transform;
}

SharedPtr<SLD::GameObject> SLD::GameObject::Create(SLD::WorldEntity& world)
{
	auto instance{ std::make_shared<GameObject>(world) };
	return instance;
}

WeakPtr<SLD::ObservePtr<SLD::RenderingComponent>> SLD::GameObject::CreateRenderingComponent(size_t elemSize,
	uint32_t elemCnt)
{
	return CreateComponent<RenderingComponent>(elemSize, elemCnt);
}

SLD::GameObjectId SLD::GameObject::GetId() const
{
	return m_Id;
}

SLD::WorldEntity& SLD::GameObject::GetWorld()
{
	return m_World;
}

SLD::GameObject::GameObject(GameObject&& other) noexcept
//: m_ComponentTable(std::move(other.m_ComponentTable))
	: m_Id(other.m_Id)
	, m_World(other.m_World)
	//, m_Transform(std::move(other.m_Transform))
{
}

SLD::GameObject& SLD::GameObject::operator=(GameObject&& other) noexcept
{
	if (this != &other)
	{
		//m_World = other.m_World;
		//m_Transform = std::move(other.m_Transform);
		//m_ComponentTable = std::move(other.m_ComponentTable);
	}

	return *this;
}

SLD::GameObject::~GameObject()
{
	//m_World.DestroyGameObject(m_Id);
}

void SLD::GameObject::RemoveGameObjectFromArchetype(const Record& oldRecord, GameObjectId id)
{
	// NOTE: This private function will delete GameObject from its archetype
	// so it's safe to assume that this pointer is valid
	SharedPtr<Archetype> clone{ oldRecord.restedArchetype.lock() };


	if (clone->gameObjectIds.size() > 1)
		WorldEntity::RelocateComponentData(oldRecord.componentStructIdx, clone);

	if (!clone->gameObjectIds.empty())
	{
		const auto willBeRemoved{ std::find(
clone->gameObjectIds.begin(),
	clone->gameObjectIds.end(),
	id) };

		if (willBeRemoved == clone->gameObjectIds.end())
			return;
		
		std::for_each(
			willBeRemoved + 1,
			clone->gameObjectIds.end(),
			[this](GameObjectId id)
			{
				Record& rec{ m_World.GetGameObjectRecord(id) };
				rec.componentStructIdx -= 1;
			});

		clone->gameObjectIds.erase(willBeRemoved);
	}

#pragma region OLD

	// This is the start address of this GameObject's component struct
	//uint8_t* gameObjectDataOffset{ archetype->componentData + rec.componentStructIdx };

	//size_t sizeOfComponentStruct{};

	//std::for_each(archetype->types.begin(), 
	//	archetype->types.end(), 
	//	[this, &sizeOfComponentStruct](const ComponentTypeId& id)
	//	{
	//		const size_t componentSize{ m_World.GetComponentSizeById(id) };
	//		sizeOfComponentStruct += componentSize;
	//	});

	//// move everything from after this GameObject
	//// to the current GameObject's address
	//std::copy_n(gameObjectDataOffset + sizeOfComponentStruct, 
	//	archetype->occupiedLength - ( rec.componentStructIdx + sizeOfComponentStruct), 
	//	gameObjectDataOffset);
	//
	//// Every GameObject after this GameObject, their stride offset
	//// will decrease by sizeofComponentStruct
	//auto thisGameObject{ std::find(archetype->gameObjectIds.begin(), archetype->gameObjectIds.end(), m_Id) };

	//std::for_each(thisGameObject,archetype->gameObjectIds.end(),
	//	[this](const GameObjectId& id)
	//	{
	//		Record& rec{ m_World.GetGameObjectRecord(id) };
	//			
	//	})

#pragma endregion 
}
