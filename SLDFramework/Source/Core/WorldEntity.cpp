#include "WorldEntity.h"
#include "../Components/RenderingComponent.h"
#include "../GameObject/GameObject.h"
#include "IdGenerator/UniqueIdGenerator.h"
#include "../Physics/System/PhysicsToTransformUpdate.h"
#include "../Physics/System/ContactFilter.h"
#include "../Physics/System/WorldToPhysicsUpdate.h"
#include "../Helpers/utils.h"
#include "../Rendering/System/RenderObject.h"
#include <iostream>
#include <box2d/box2d.h>
#include "../Tracer/minitrace.h"


SLD::WorldEntity::WorldEntity()
	: m_TickComponent()
	//, m_RenderComponents()
	, m_SystemMap()
	, m_ReadBufferIndex(ObjectBuffer::Read)
	, m_CopyBufferIndex(ObjectBuffer::Copy)
	, m_RenderData(1024)
	, m_RenderBuffer(1024)
	, m_TickTasks()
	, m_GameObjectCounter(1)
	, m_ShouldUpdateArchetypeList()
{
	m_TickTasks.reserve(size_t(TickComponent::Type::Count));

	CreatePhysicsWorld();

	AddSystem<PhysicsToTransformUpdate>(*this);
	AddSystem<WorldToPhysicsUpdate>(*this);
	//AddSystem<AsyncTransformUpdate>(*this);

	m_CopyObjectBufferThread.Start();
	m_CopyObjectBufferThread.AssignTask(&WorldEntity::CopyFromWriteBufferToCopyBuffer, *this);
	m_RenderObjects.reserve(100);
}

SLD::WorldEntity::~WorldEntity()
{

}

SLD::RenderBuffer& SLD::WorldEntity::GetRenderBuffer()
{
	return m_RenderBuffer;
}

SharedPtr<SLD::ObservePtr<SLD::RenderingComponent>> SLD::WorldEntity::AllocRenderingComponent(
	const SharedPtr<ObservePtr<TransformComponent>>&, size_t elemSize, uint32_t)
{
	constexpr const char* uniqueId{ RenderingComponent::UniqueId };
	auto it{ m_NonTickComponent.try_emplace(uniqueId,NonTickComponentPool{}) };
	LoggingResource& logResource{ it.first->second.logger };

	// NOTE: I don't know I have to reassign this again so it works
	logResource = LoggingResource{ &it.first->second.resource };
	auto& realResource{ it.first->second.resource };

	void* allocPtr{ logResource.do_allocate(sizeof(RenderingComponent),alignof(RenderingComponent)) };

	// Handle Render Buffer
	SharedPtr<uint8_t> pointerToBuffer{};
	{
		void* renderPointer{ m_RenderData.resource.do_allocate(elemSize, alignof(std::max_align_t)) };
		uint8_t* castPointer{ static_cast<uint8_t*>(renderPointer) };
		pointerToBuffer = SharedPtr<uint8_t>{ castPointer, [this, elemSize](uint8_t* ptr)
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
		//new (allocPtr) RenderingComponent{ transform,pointerToBuffer,bufferObserver,elemSize,elemCnt };
	}



	auto& bufferHead{ realResource.GetBufferHead() };
	const size_t offSetFromHead{ size_t(std::abs(bufferHead - (uint8_t*)allocPtr)) };

	ObservePtr<RenderingComponent>* ob{ new ObservePtr<RenderingComponent>{bufferHead,offSetFromHead} };

	SharedPtr<ObservePtr<RenderingComponent>> out{ ob,[this,&logResource](ObservePtr<RenderingComponent>* ptr)
	{
			//ptr->GetPtr()->~RenderingComponent();
			uint8_t* temp{ptr->GetPtrPointTo()};
			if (ptr->GetPtr())
				ptr->GetPtr()->~RenderingComponent();
			//delete ptr;
			logResource.do_deallocate(temp,sizeof(RenderingComponent),alignof(RenderingComponent));
			m_RenderData.totalElement--;
			delete ptr;
			ptr = nullptr;
		} };

	m_RenderData.totalElement++;

	return out;
}

//SharedPtr<SLD::RenderingComponent> SLD::WorldEntity::AllocRenderComponent(const SharedPtr<ObservePtr<TransformComponent>>& transform, size_t elemSize,
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
//	return SharedPtr<RenderingComponent>{&emplace, [](RenderingComponent* ptr)
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

float SLD::WorldEntity::GetDeltaTime() const
{
	return 0;
}


//SharedPtr<SLD::RenderingComponent> SLD::WorldEntity::AllocRenderComponent(size_t elemSize, uint32_t elemCnt)
//{
//	auto& component{ m_RenderComponents.emplace_back(elemSize,elemCnt) };
//	SharedPtr<RenderingComponent> smart{ &component,[](RenderingComponent* component)
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

SLD::InputSettings& SLD::WorldEntity::GetWorldInputSetting()
{
	return m_InputSetting;
}


std::pair<bool, WeakPtr<SLD::Archetype>> SLD::WorldEntity::GetArchetype(const std::initializer_list<ComponentTypeId>& archetypeId)
{
	const std::string id{ ArchetypeIdsToKey(archetypeId) };

	auto& archetypeBuffer{ m_ArchetypeBuffer[ObjectBuffer::Write] };

	if (const auto fIt{ archetypeBuffer.find(id) };
		fIt != archetypeBuffer.end())
	{
		return std::make_pair(true, fIt->second);
	}

	SharedPtr<Archetype> archetype = std::make_shared<Archetype>();
	archetypeBuffer[id] = archetype;

	return std::make_pair(false, archetype);
}

std::pair<bool, WeakPtr<SLD::Archetype>> SLD::WorldEntity::GetArchetype(
	const std::vector<ComponentTypeId>& archetypeId)
{
	const std::string id{ ArchetypeIdsToKey(archetypeId) };

	auto& archetypeBuffer{ m_ArchetypeBuffer[ObjectBuffer::Write] };

	if (const auto fIt{ archetypeBuffer.find(id) };
		fIt != archetypeBuffer.end())
	{
		return std::make_pair(true, fIt->second);
	}

	SharedPtr<Archetype> archetype = std::make_shared<Archetype>();
	archetypeBuffer[id] = archetype;

	return std::make_pair(false, archetype);
}

std::pair<bool, WeakPtr<SLD::Archetype>> SLD::WorldEntity::GetArchetype(const std::string& combinedArchetypeId)
{
	auto& archetypeBuffer{ m_ArchetypeBuffer[ObjectBuffer::Write] };

	if (const auto fIt{ archetypeBuffer.find(combinedArchetypeId) };
		fIt != archetypeBuffer.end())
	{
		return std::make_pair(true, fIt->second);
	}

	SharedPtr<Archetype> archetype = std::make_shared<Archetype>();
	archetypeBuffer[combinedArchetypeId] = archetype;

	return std::make_pair(false, archetype);
}

void SLD::WorldEntity::OnStartFrame()
{
	MTR_SCOPE("main", "OnStartFrame");
	m_CopyObjectBufferThread.WaitTillFinished();
}

void SLD::WorldEntity::OnInputValidation(const SharedPtr<Window>& window)
{
	MTR_SCOPE("main", "OnInputValidation");
	InputManager::QueryUserInputsAsyncMaybe(window, m_InputSetting.m_MainKeyPool);
}

void SLD::WorldEntity::OnValidation()
{
	MTR_SCOPE("main", "OnValidation");
	if (m_ShouldUpdateArchetypeList)
		m_CopyObjectBufferThread.Wake();
#pragma region OLD

	//for (auto& [archetypeIds, gameObjects] : m_PendingValidateGameObjs)
	//{
	//	for (const auto& id : gameObjects)
	//	{
	//		//if (!shouldDestroy)
	//		//{
	//		//	// add GameObject id to the correspond archetype
	//		//	SharedPtr<Archetype> targetArchetype{ m_ArchetypesMap[archetypeIds] };
	//		//	targetArchetype->gameObjectIds.emplace_back(id);
	//		//	// DONE !
	//		//}

	//			// Do some magic to organize component data
	//			// 1. Get the record of this GameObject
	//			// NOTE: we can safely assume that all GameObject in this list has an archetype
	//		const auto& matchedItr{ m_GameObjectToArchetypeMap.find(id) };

	//		// 2. Because the data in component data array will be missing
	//		// we need to relocate all the data after the given GameObject up 1 index
	//		SharedPtr<Archetype> targetArchetype{ m_ArchetypesMap[archetypeIds] };

	//		RelocateComponentData(matchedItr->second.componentStructIdx, targetArchetype);

	//		// 3. All GameObject after this one, their component index will decrease by 1
	//		const auto willBeRemoved{ std::find(
	//			targetArchetype->gameObjectIds.begin(),
	//			targetArchetype->gameObjectIds.end(),
	//			id) };

	//		std::for_each(
	//			willBeRemoved + 1,
	//			targetArchetype->gameObjectIds.end(),
	//			[this](GameObjectId id)
	//			{
	//				Record& rec{ m_GameObjectToArchetypeMap.at(id) };
	//				rec.componentStructIdx -= 1;
	//			});

	//		// 4. Delete GameObject from the list
	//		targetArchetype->gameObjectIds.erase(willBeRemoved);

	//		// 5. Delete GameObject from record map
	//		m_GameObjectToArchetypeMap.erase(matchedItr);
	//	}

	//	gameObjects.clear();
	//}

#pragma endregion 
}

void SLD::WorldEntity::OnPreRenderUpdate(float dt)
{
	MTR_SCOPE("main", "OnPreRenderUpdate");
	UpdatePipeline(PipelineLayer::OnPreRender, dt);
}

void SLD::WorldEntity::OnPreAsyncUpdate(float dt)
{
	MTR_SCOPE("main", "OnPreAsyncUpdate");
	UpdatePipeline(PipelineLayer::OnPreAsync, dt);
}

void SLD::WorldEntity::OnAsyncUpdate(float dt)
{
	UpdatePipeline(PipelineLayer::OnAsync,dt);

	// Fetch future result
	for (const auto& system : 
		m_SystemMap[size_t(PipelineLayer::OnAsync)])
	{
		system->FetchFutureResults();
	}
}

void SLD::WorldEntity::OnPhysicsValidation(float dt)
{
	MTR_SCOPE("main", "OnPhysicsValidation");
	
	UpdatePipeline(PipelineLayer::OnFixedUpdate, dt);

	UpdatePipeline(PipelineLayer::OnFixedUpdateAsync, dt);

	for (const auto& system : 
		m_SystemMap[size_t(PipelineLayer::OnFixedUpdateAsync)])
	{
		system->FetchFutureResults();
	}

	UpdatePipeline(PipelineLayer::OnPrePhysicsStep, dt);
	
	m_PhysicsWorld->Step(dt, 6, 2);

	UpdatePipeline(PipelineLayer::OnPostPhysicsStep, dt);
}


void SLD::WorldEntity::OnRenderSystem(const SharedPtr<Window>& winHandle)
{
	MTR_SCOPE("main", "OnRenderSystem");
	RenderPipeline(winHandle, PipelineLayer::OnRender);
}

void SLD::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>& system)
{
	m_PendingDeleteSystem.emplace_back(std::make_pair(system->GetLayer(), system));
}

void SLD::WorldEntity::DestroyGameObject(GameObjectId id)
{
	m_ToDestroyGameObjects.emplace_back(id);

#pragma region OLD
	//Record& rec{ m_GameObjectToArchetypeMap.at(id) }; // i know that every GameObject creation will have the record object

	//ArchetypeRecord archetypeRec{};

	//// 1. Check if old archetype exist
	//if (const auto checkPtr{ rec.restedArchetype };
	//	checkPtr.lock())
	//{
	//	// 1.1 if exist then clone that archetype
	//	SharedPtr<Archetype> clone{ checkPtr.lock() };

	//	// NOTE: Since we using pool type data we don't need to free anything
	//	// just delete the GameObject and it's fine
	//	// 
	//	// 1.2 Delete componentStruct in that archetype
	//	//uint8_t* deleteAddress clone->componentData[rec.componentStructIdx * clone->sizeOfStruct];
	//	//clone->componentsData.erase(
	//	//	std::find(
	//	//		clone->componentsData.begin(),
	//	//		clone->componentsData.end(),
	//	//		clone->componentsData[rec.componentStructIdx]));

	//	// 1.3 find where that GameObject is in GameObjectIds vector
	//	const auto willBeDeleted{ std::find(
	//		clone->gameObjectIds.begin(),
	//		clone->gameObjectIds.end(),
	//		id) };


	//	// 1.4 Change all recorded GameObject's componentStructIdx after deleted GameObject
	//	std::for_each(
	//		willBeDeleted + 1,
	//		clone->gameObjectIds.end(),
	//		[this](GameObjectId id)
	//		{
	//			Record& rec{ m_GameObjectToArchetypeMap.at(id) };
	//			rec.componentStructIdx -= 1;
	//		});

	//	clone->gameObjectIds.erase(willBeDeleted);

	//	// 1.5 assign cloned archetype to pending validate archetype
	//	archetypeRec.archetypeCopy = clone;
	//	archetypeRec.combinedTypes = ArchetypeIdsToKey(clone->types);

	//	//AddArchetypeForValidation(archetypeRec);
	//}

	//// 2. If not then just delete from GameObject map
	////m_GameObjectToArchetypeMap.erase(m_GameObjectToArchetypeMap.find(id));
#pragma endregion 
}

bool SLD::WorldEntity::IsArchetypeExist(const std::vector<ComponentTypeId>& archetypeId)
{
	const std::string archetypeIdHash{ ArchetypeIdsToKey(archetypeId) };

	const auto& writeArchetypeMap{ m_ArchetypeBuffer[ObjectBuffer::Write] };

	if (writeArchetypeMap.find(archetypeIdHash) !=
		writeArchetypeMap.end())
	{
		return true;
	}

	return false;
}

SLD::GameObjectId SLD::WorldEntity::CreateGameObject()
{
	return 0;
}

void SLD::WorldEntity::RegisterGameObject(const SharedPtr<GameObject>& instance)
{
	m_GameObjectToArchetypeMap[instance->GetId()];
}

void SLD::WorldEntity::RegisterGameObject(GameObjectId id)
{
	m_GameObjectToArchetypeMap[id];
}

SharedPtr<SLD::BaseComponent> SLD::WorldEntity::GetRegisterComponent(ComponentTypeId id)
{
	return m_ComponentMap.at(id).instance;
}

bool SLD::WorldEntity::ContainsGameObjectRecord(GameObjectId id)
{
	if (m_GameObjectToArchetypeMap.find(id) != m_GameObjectToArchetypeMap.end())
	{
		return true;
	}

	return false;
}

size_t SLD::WorldEntity::GetComponentSizeById(ComponentTypeId id)
{
	return m_ComponentMap.at(id).size;
}

SLD::Record& SLD::WorldEntity::GetGameObjectRecord(GameObjectId id)
{
	return m_GameObjectToArchetypeMap.at(id);
}

SLD::GameObjectId SLD::WorldEntity::GetNewId()
{
	return m_GameObjectCounter++;
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


std::string SLD::WorldEntity::ArchetypeIdsToKey(const std::initializer_list<ComponentTypeId>& ids)
{
	// Assume that the archetypeId has already been sorted
	// ex. 1:2:4, 3:4, 2:4

	std::stringstream keyStream{};

	std::for_each(ids.begin(), ids.end(), [&keyStream](const ComponentTypeId id)
		{
			keyStream << id << '-';
		});

	//return size_t(std::stoi(keyStream.str()));
	return keyStream.str();
}

std::string SLD::WorldEntity::ArchetypeIdsToKey(const std::vector<ComponentTypeId>& ids)
{
	// Assume that the archetypeId has already been sorted
	// ex. 1:2:4, 3:4, 2:4

	//int count{ int(powf(10,float(ids.size() - 1))) };
	std::stringstream keyStream{};

	std::for_each(ids.begin(), ids.end(), [&keyStream](const ComponentTypeId id)
		{
			keyStream << id << '-';
		});

	//return size_t(std::stoi(keyStream.str()));
	return keyStream.str();
}

bool SLD::WorldEntity::IsArchetypeIdsSameAsKey(const std::string& key, const std::string& combinedKey)
{
	std::vector<size_t> checker{};
	std::vector<size_t> checkee{};

	std::stringstream keySS{};

	for (const auto& ch : key)
	{
		if (ch != '-')
		{
			keySS << ch;
		}
		else
		{
			size_t num{};
			keySS >> num;
			checker.push_back(num);
			keySS.str(std::string{});
			keySS.clear();
		}
	}

	keySS.str(std::string{});
	keySS.clear();

	for (const auto& ch : combinedKey)
	{
		if (ch != '-')
		{
			keySS << ch;
		}
		else
		{
			size_t num{};
			keySS >> num;
			checkee.push_back(num);
			keySS.str(std::string{});
			keySS.clear();
		}
	}

	for (const auto& id : checker)
	{
		if (std::find(checkee.begin(), checkee.end(), id) ==
			checkee.end())
		{
			return false;
		}
	}


	return true;
}

void SLD::WorldEntity::RelocateComponentData(int indexInComponent, const SharedPtr<Archetype>& archetype)
{
	if (size_t(indexInComponent) >= archetype->gameObjectIds.size() - 1)
		return;

	const size_t deleteIndex{ size_t(indexInComponent) };
	const size_t nextIndex{ size_t(indexInComponent + 1) };
	const size_t totalOccupiedSize{ size_t(archetype->sizeOfStruct) * archetype->gameObjectIds.size() };
	const size_t sizeUntilGameObject{ size_t(archetype->sizeOfStruct) * nextIndex };

	// totalOccupiedSize - sizeUntilGameObject
	const size_t totalOccupiedSizeAfterSpecifiedGameObject{ totalOccupiedSize - sizeUntilGameObject };

	// address at the end of all GameObject' components
	uint8_t* moveEndAddress{ &archetype->componentData[deleteIndex * size_t(archetype->sizeOfStruct)] };
	uint8_t* moveStartAddress{ &archetype->componentData[nextIndex * size_t(archetype->sizeOfStruct)] };

	//std::memset(moveEndAddress,0,size_t(archetype->sizeOfStruct));
	
	// copy over all data up 1 index
	std::copy_n(
		moveStartAddress,
		totalOccupiedSizeAfterSpecifiedGameObject,
		moveEndAddress
	);
}

void SLD::WorldEntity::RelocateComponentData(WorldEntity& world, int indexInComponent,
	const SharedPtr<Archetype>& archetype)
{
	if (size_t(indexInComponent) >= archetype->gameObjectIds.size() - 1)
		return;

	const size_t totalOccupiedSize{ size_t(archetype->sizeOfStruct) * archetype->gameObjectIds.size() };
	const size_t sizeUntilGameObject{ size_t(archetype->sizeOfStruct) * size_t(indexInComponent + 1) };

	// totalOccupiedSize - sizeUntilGameObject
	const size_t totalOccupiedSizeAfterSpecifiedGameObject{ totalOccupiedSize - sizeUntilGameObject };

	// address at the end of all GameObject' components
	uint8_t* moveEndAddress{ &archetype->componentData[indexInComponent * archetype->sizeOfStruct] };
	uint8_t* moveStartAddress{ &archetype->componentData[(indexInComponent + 1) * archetype->sizeOfStruct] };

	std::memset(moveEndAddress, 0, size_t(archetype->sizeOfStruct));

	// Test
	for (size_t i = indexInComponent + 1; i < archetype->gameObjectIds.size(); ++i)
	{
		size_t offset{};
		for (const auto& compId : archetype->types)
		{
			world.GetRegisterComponent(compId)->MoveData(moveStartAddress + offset, moveEndAddress + offset);
			offset += world.GetComponentSizeById(compId);
		}
	}
	
	 //copy over all data up 1 index
	//std::copy_n(
	//	moveStartAddress,
	//	totalOccupiedSizeAfterSpecifiedGameObject,
	//	moveEndAddress
	//);
}

const SharedPtr<b2World>& SLD::WorldEntity::GetPhysicsWorld() const
{
	return m_PhysicsWorld;
}

void SLD::WorldEntity::AddPhysiscsBodyDef(const PhysicsUserData& data)
{
	if (std::find(m_PhysicsRuntimeData.begin(), m_PhysicsRuntimeData.end(), data)
		== m_PhysicsRuntimeData.end())
	{
		m_PhysicsRuntimeData.emplace_back(data);
	}
}

size_t SLD::WorldEntity::GetOffsetOfComponent(const SharedPtr<Archetype>& archetype, size_t componentIdx)
{
	size_t offset{};

	for (const auto& typeId : archetype->types)
	{
		if (typeId == componentIdx)
			break;

		offset += m_ComponentMap.at(typeId).size;
	}

	return offset;
}

void SLD::WorldEntity::UpdatePipeline(PipelineLayer layer, float deltatime)
{
	for (const auto& system : m_SystemMap[size_t(layer)])
	{
		for (const auto& [type, archetype] : m_ArchetypeBuffer[ObjectBuffer::Write])
		{
			if (!archetype->gameObjectIds.empty())
			{
				if (IsArchetypeIdsSameAsKey(system->GetKey(), type))
				{
					system->InternalUpdate(deltatime, archetype);
				}
			}
		}
	}

	m_ShouldUpdateArchetypeList = true;
}

void SLD::WorldEntity::RenderPipeline(const SharedPtr<Window>& window, PipelineLayer layer)
{
	//std::vector<RenderObject> validRenderObject{};
	//validRenderObject.reserve(50);

	for (const auto& system : m_SystemMap[size_t(layer)])
	{
		for (const auto& [type, archetype] : m_ArchetypeBuffer[m_ReadBufferIndex])
		{
			if (!archetype->gameObjectIds.empty())
			{
				if (IsArchetypeIdsSameAsKey(system->GetKey(), type))
				{
					int count{};
					for (const auto& id : archetype->gameObjectIds)
					{
						RenderObject rObject{};
						rObject.indexInStruct = count++;
						rObject.parent = archetype;
						rObject.gameId = id;

						m_RenderObjects.emplace_back(rObject);
					}

					//validArchetype.emplace_back(archetype);
					//validGameObject.insert(validGameObject.end(), archetype->gameObjectIds.begin(), archetype->gameObjectIds.end());
					//system->InternalRender(window, archetype);
				}
			}
		}

		if (!m_RenderObjects.empty())
		{
			system->InternalRender(window, m_RenderObjects);
			m_RenderObjects.clear();
		}
	}
}



void SLD::WorldEntity::CreatePhysicsWorld()
{
	const b2Vec2 worldGravity{ 0.0f,0.0f };
	m_PhysicsWorld = std::make_shared<b2World>(worldGravity);

	m_PhysicsContactFilter = std::make_shared<ContactFilter>(*this);
}

void SLD::WorldEntity::DestroyPendingGameObject()
{
	for (const auto& id : m_ToDestroyGameObjects)
	{
		if (const auto& matchedItr{ m_GameObjectToArchetypeMap.find(id) };
			matchedItr != m_GameObjectToArchetypeMap.end())
		{
			std::cout << "Destroy : " << id << '\n';
			// Do some magic to organize component data
			// 2. Because the data in component data array will be missing
			// we need to relocate all the data after the given GameObject up 1 index
			const SharedPtr<Archetype> targetArchetype{ matchedItr->second.restedArchetype.lock() };

			if (targetArchetype)
			{
				// Call destructor on components
				uint8_t* componentAddress{ &targetArchetype->componentData[
					matchedItr->second.componentStructIdx * targetArchetype->sizeOfStruct] };
				
				for (const auto& type : targetArchetype->types)
				{
					const auto& [component, size] {m_ComponentMap.at(type)};
					component->DestroyData(componentAddress);
					componentAddress += size;
				}

				if (targetArchetype->gameObjectIds.size() > 1)
					RelocateComponentData(matchedItr->second.componentStructIdx, targetArchetype);

				// 3. All GameObject after this one, their component index will decrease by 1
				const auto willBeRemoved{ std::find(
					targetArchetype->gameObjectIds.begin(),
					targetArchetype->gameObjectIds.end(),
					id) };

				std::for_each(
					willBeRemoved + 1,
					targetArchetype->gameObjectIds.end(),
					[this](GameObjectId id)
					{
						Record& rec{ m_GameObjectToArchetypeMap.at(id) };
						rec.componentStructIdx -= 1;
					});

				// 4. Delete GameObject from the list
				targetArchetype->gameObjectIds.erase(willBeRemoved);

				// 5. Delete GameObject from record map
				m_GameObjectToArchetypeMap.erase(matchedItr);

				m_ShouldUpdateArchetypeList = true;
			}
		}
	}

	if (!m_ToDestroyGameObjects.empty())
		m_ToDestroyGameObjects.clear();
}

void SLD::WorldEntity::ValidateSystems()
{
	for (const auto& [pipeline, system] : m_PendingAddSystem)
	{
		auto& vectorSystem{ m_SystemMap[size_t(pipeline)] };

		const auto fIter = std::find(
			vectorSystem.begin(),
			vectorSystem.end(),
			system);

		if (fIter == vectorSystem.end())
			vectorSystem.emplace_back(system);
	}

	for (const auto& [pipeline, system] : m_PendingDeleteSystem)
	{
		auto& vectorSystem{ m_SystemMap[size_t(pipeline)] };

		const auto fIter = std::find(
			vectorSystem.begin(),
			vectorSystem.end(),
			system);

		if (fIter != vectorSystem.end())
			vectorSystem.erase(fIter);
	}

	if (!m_PendingAddSystem.empty())
		m_PendingAddSystem.clear();
	if (!m_PendingDeleteSystem.empty())
		m_PendingDeleteSystem.clear();

}

void SLD::WorldEntity::ValidateArchetype()
{
	for (const auto& id : m_ToAddGameObjects)
	{
		Record& rec{ m_GameObjectToArchetypeMap.at(id) };
		if (const auto archetype{ rec.restedArchetype.lock() })
		{
			if (std::find(archetype->gameObjectIds.begin(), archetype->gameObjectIds.end(), id)
				== archetype->gameObjectIds.end())
			{
				archetype->gameObjectIds.emplace_back(id);
			}
		}
	}

	for (const auto& moveData : m_ToMoveGameObjects)
	{
		// 1. Check if there's enough space for our new component
		Record& newRec{ m_GameObjectToArchetypeMap.at(moveData.id) };
		const SharedPtr<Archetype> newArchetype{ newRec.restedArchetype.lock() };
		const SharedPtr<Archetype>& deleteArchetype{ moveData.oldArchetype };

		if (newArchetype && deleteArchetype)
		{
			AllocateAndMove(*this, newArchetype);

			// move data from old to new archetype
			uint8_t* oldDataAddress{ &deleteArchetype->componentData[
			moveData.oldCompIndex * deleteArchetype->sizeOfStruct] };

			uint8_t* newDataAddress{ &newArchetype->componentData[
			newRec.componentStructIdx * newArchetype->sizeOfStruct] };

			size_t newOffset{};
			size_t oldOffset{};
			for (const auto& typeId : deleteArchetype->types)
			{
				const auto& registerComponent{ GetRegisterComponent(typeId) };

				if (typeId != moveData.componentId)
				{
					registerComponent->MoveData(oldDataAddress + oldOffset, newDataAddress + newOffset);
					newOffset += GetComponentSizeById(typeId);
				}
				else
					registerComponent->DestroyData(oldDataAddress + oldOffset);

				oldOffset += GetComponentSizeById(typeId);
			}


			// move data up one index
			if (deleteArchetype->gameObjectIds.size() > 1)
				RelocateComponentData(int(moveData.oldCompIndex), deleteArchetype);

			deleteArchetype->gameObjectIds.erase(
				std::find(
					deleteArchetype->gameObjectIds.begin(),
					deleteArchetype->gameObjectIds.end(),
					moveData.id));
		}

	}

	if (!m_ToAddGameObjects.empty())
		m_ToAddGameObjects.clear();

	if (!m_ToMoveGameObjects.empty())
		m_ToMoveGameObjects.clear();
}

void SLD::WorldEntity::ValidatePhysicsData()
{
	for (auto& data : m_PhysicsRuntimeData)
	{
		auto t = SharedPtr<b2Body>{ m_PhysicsWorld->CreateBody(&data.bodyDef),[this](b2Body* ptr)
		{
			if (ptr)
				m_PhysicsWorld->DestroyBody(ptr);
		} };

		t->CreateFixture(&data.fixtureDef);
		GameObjectId owner{ data.bodyDef.userData.pointer };
		GetComponent<Box2DComponent>(owner)->SetPhysicsBody(t);
	}

	if (!m_PhysicsRuntimeData.empty())
		m_PhysicsRuntimeData.clear();
}

void SLD::WorldEntity::CopyFromWriteBufferToCopyBuffer()
{
#pragma region NOTWORKING
	//for (const auto& [type, archetype] : m_ArchetypeBuffer[m_ReadBufferIndex])
	//{
	//	// copy over iteration data
	//	int count{};
	//	for (const auto& id : archetype->gameObjectIds)
	//	{
	//		uint8_t* readDataStart{ archetype->componentData.get() + (archetype->sizeOfStruct * count) };
	//		Record& writeRec{ GetGameObjectRecord(id) };
	//		const SharedPtr<Archetype> writeArchetype{ writeRec.restedArchetype.lock() };

	//		uint8_t* writeDataStart{ &writeArchetype->componentData[
	//			size_t(writeRec.componentStructIdx) * size_t(writeArchetype->sizeOfStruct)] };


	//		size_t oldOffset{};

	//		for (const auto& compId : archetype->types)
	//		{
	//			const size_t sizeOfComp{ GetComponentSizeById(compId) };
	//			
	//			const auto findIter{
	//				std::find(
	//					writeArchetype->types.begin(),
	//					writeArchetype->types.end(),
	//					compId) };

	//			if(findIter != writeArchetype->types.end())
	//			{
	//				size_t newOffset{};
	//				std::for_each(writeArchetype->types.begin(), findIter, [this, &newOffset](const GameObjectId& cId)
	//					{
	//						newOffset += GetComponentSizeById(cId);
	//					});

	//				uint8_t* copyStart{ readDataStart + oldOffset };
	//				uint8_t* copyEnd{ writeDataStart + newOffset };

	//				std::copy_n(copyStart,
	//					sizeOfComp,
	//					copyEnd);
	//				
	//				oldOffset += sizeOfComp;
	//			}
	//			
	//		}
	//		++count;
	//	}
	//}
#pragma endregion
	
	int trace{};
	MTR_START("Async", "CopyFromWriteBufferToCopyBuffer", &trace);
	
	ValidateSystems();
	//ValidateArchetype();
	//ValidatePhysicsData();
	DestroyPendingGameObject();

	auto& writeBuffer{ m_ArchetypeBuffer[ObjectBuffer::Write] };
	auto& copyBuffer{ m_ArchetypeBuffer[m_CopyBufferIndex] };

	//GameStateComponent* temp{ GetComponent<GameStateComponent>(1) };
	//if(temp)
	//	std::cout << size_t(temp->currentState) << std::endl;

	// JUST DO IT!!
	copyBuffer.clear();

	// 1. Delete any archetype that has no GameObject in it
	for (auto it = writeBuffer.begin(); it != writeBuffer.end();)
	{
		if (it->second->gameObjectIds.empty())
		{
			it = writeBuffer.erase(it);
		}
		else
			++it;
	}

	// clone
	for (const auto& [type, archetype] : writeBuffer)
	{
		copyBuffer[type] = archetype->Clone();
	}

	MTR_FINISH("Async", "CopyFromWriteBufferToCopyBuffer", &trace);
	trace;

}

void SLD::WorldEntity::SwapCopyBufferToReadBuffer()
{
	if (m_ShouldUpdateArchetypeList)
		std::swap(m_ReadBufferIndex, m_CopyBufferIndex);
}

void SLD::WorldEntity::CleanArchetypeMarked()
{
	m_ShouldUpdateArchetypeList = false;
}

void SLD::WorldEntity::MarkArchetypeMapDirty()
{
	m_ShouldUpdateArchetypeList = true;
}

void SLD::WorldEntity::AddGameObjectToWaitingList(GameObjectId id)
{
	if (std::find(m_ToAddGameObjects.begin(), m_ToAddGameObjects.end(), id)
		== m_ToAddGameObjects.end())
	{
		m_ToAddGameObjects.emplace_back(id);
	}
}

SLD::PersistentThreadWorker& SLD::WorldEntity::EmplaceNewWorker(const std::string& id)
{
	auto& thread{ m_TickTasks.emplace_back(TickTask{ id,{} }).worker };
	thread.Start();
	return thread;
}


