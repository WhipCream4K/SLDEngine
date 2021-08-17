
#ifndef SLD_FRAMEWORK_SFMLRENDERSYSTEM
#define SLD_FRAMEWORK_SFMLRENDERSYSTEM

#include "../System/RenderSystem.h"
#include "../../Core/WorldEntity.h"

namespace SLD
{
	template<typename ...Args>
	class SFML2DRenderSystem : public RenderSystem<Args...>
	{
	public:

		SFML2DRenderSystem(WorldEntity& world);

		virtual ~SFML2DRenderSystem() override = default;

		virtual void OnRender(const SharedPtr<Window>& renderWindow, GameObjectId id, const TransformComponent*, Args*...) override = 0;
		virtual void InternalRender(const SharedPtr<Window>& renderWindow,  std::vector<RenderObject>& validRenderObjects) override;
	
	protected:

		//virtual void CallSystemRenderLoop(
		//	std::vector<ComponentTypeId>& gameObjectIds,
		//	const SharedPtr<Window>& winHandle,
		//	const std::vector<size_t>& strideOffsets,
		//	uint8_t* componentAddress,
		//	int componentStructSize) override;

	private:

		int m_ResourceSize{};
		OwnedPtr<uint8_t[]> m_SwapResource;
		std::vector<size_t> m_NumKeys{};
	};

	template <typename ... Args>
	SFML2DRenderSystem<Args...>::SFML2DRenderSystem(WorldEntity& world)
		: RenderSystem<Args...>(world)
		, m_ResourceSize(1024)
		, m_SwapResource(std::make_unique<uint8_t[]>(m_ResourceSize))
	{
		std::stringstream keySS{};
		for (const auto& ch : this->m_Key)
		{
			if (ch != '-')
				keySS << ch;
			else
			{
				size_t num{};
				keySS >> num;
				m_NumKeys.push_back(num);
				keySS.str(std::string{});
				keySS.clear();
			}
		}
	}

	//template <typename ... Args>
	//void SFML2DRenderSystem<Args...>::InternalRender(const SharedPtr<Window>& renderWindow,
	//	std::vector<GameObjectId>& validGameObjectIds)
	//{
	//	WorldEntity& world{ this->m_World };

	//	if (validGameObjectIds.size() > 1)
	//	{
	//		std::sort(validGameObjectIds.begin(), validGameObjectIds.end(), [&world](const GameObjectId left,const GameObjectId right)
	//			{
	//				TransformComponent* transformLeft = world.GetComponent<TransformComponent>(left);
	//				TransformComponent* transformRight = world.GetComponent<TransformComponent>(right);

	//				return transformLeft->GetWorldPos().z < transformRight->GetWorldPos().z;
	//			});
	//	}

	//	
	//	for (const auto& id : validGameObjectIds)
	//	{
	//		Record& rec{ world.GetGameObjectRecord(id) };

	//		// TODO: This is the archetype from write buffer which is still in work
	//		// FIX THIS!  
	//		const SharedPtr<Archetype> archetype{ rec.restedArchetype.lock() };
	//		
	//		uint8_t* componentAddress{&archetype->componentData[
	//			size_t(rec.componentStructIdx) * archetype->sizeOfStruct]};


	//		std::vector<size_t> strideOffsets{};

	//		int nextKey{};
	//		size_t offset{};
	//		for (const auto& compId : archetype->types)
	//		{
	//			if (compId == m_NumKeys[nextKey])
	//			{
	//				strideOffsets.push_back(offset);
	//				++nextKey;
	//				if(nextKey == int(m_NumKeys.size()))
	//					break;
	//			}

	//			offset += world.GetComponentSizeById(compId);
	//		}

	//		this->ApplyPack(id, strideOffsets, renderWindow, componentAddress, std::index_sequence_for<Args...>{});

	//	}
	//	
	//}

	template <typename ... Args>
	void SFML2DRenderSystem<Args...>::InternalRender(const SharedPtr<Window>& renderWindow,
		std::vector<RenderObject>& validRenderObjects)
	{
		WorldEntity& world{ this->m_World };

		if (validRenderObjects.size() > 1)
		{
			std::sort(validRenderObjects.begin(), validRenderObjects.end(), [](const RenderObject& left, const RenderObject& right)
				{
					TransformComponent* posLeft{
						reinterpret_cast<TransformComponent*>(
							&left.parent->componentData[
								left.indexInStruct * left.parent->sizeOfStruct]) };

					TransformComponent* posRight{ reinterpret_cast<TransformComponent*>(
								&right.parent->componentData[
									right.indexInStruct * right.parent->sizeOfStruct]) };

					return posLeft->GetWorldPos().z < posRight->GetWorldPos().z;
				});
		}

		for (const auto& object : validRenderObjects)
		{
			SharedPtr<Archetype> archetype{ object.parent };
			
			uint8_t* componentAddress{ &archetype->componentData[
				object.indexInStruct * archetype->sizeOfStruct] };

			std::vector<size_t> strideOffsets{};

			int nextKey{};
			size_t offset{};
			for (const auto& compId : archetype->types)
			{
				if (compId == m_NumKeys[nextKey])
				{
					strideOffsets.push_back(offset);
					++nextKey;
					if (nextKey == int(m_NumKeys.size()))
						break;
				}

				offset += world.GetComponentSizeById(compId);
			}

			this->ApplyPack(object.gameId, strideOffsets, renderWindow, componentAddress, std::index_sequence_for<Args...>{});
		}
	}

	//template <typename ... Args>
	//void SFML2DRenderSystem<Args...>::CallSystemRenderLoop(std::vector<ComponentTypeId>& gameObjectIds,
	//	const SharedPtr<Window>& winHandle, const std::vector<size_t>& strideOffsets, uint8_t* componentAddress,
	//	int componentStructSize)
	//{
	//	// 1. copy GameObjects for reference
	//	std::vector<GameObjectId> auxiliary{ gameObjectIds };

	//	// 2. sort all the GameObjectsIds
	//	std::sort(gameObjectIds.begin(), gameObjectIds.end(), [this](const GameObjectId& left, const GameObjectId& right)
	//		{
	//			WorldEntity& world{ this->m_World };
	//			TransformComponent* transformLeft = world.GetComponent<TransformComponent>(left);
	//			TransformComponent* transformRight = world.GetComponent<TransformComponent>(right);

	//			return transformLeft->GetWorldPos().z < transformRight->GetWorldPos().z;
	//		});

	//	// 3. using the copy find the correspond element then move the data according to that position
	//	size_t index{};
	//	if (componentStructSize > m_ResourceSize)
	//	{
	//		m_SwapResource = std::move(std::make_unique<uint8_t[]>(componentStructSize));
	//		m_ResourceSize = componentStructSize;
	//	}

	//	if(gameObjectIds.size() > 1)
	//	{
	//		for (size_t i = 0; i < gameObjectIds.size(); ++i)
	//		{
	//			const auto findItr{ std::find(
	//				gameObjectIds.begin(),
	//				gameObjectIds.end(),
	//					auxiliary[index]) };

	//			const size_t distanceFromBegin{ (size_t)std::distance(
	//				gameObjectIds.begin(),
	//				findItr) };

	//			if (i == distanceFromBegin)
	//			{
	//				++index;
	//				continue;
	//			}

	//			uint8_t* oldAddress{};
	//			if (i == 0)
	//				oldAddress = &componentAddress[0];
	//			else
	//				oldAddress = &m_SwapResource.get()[index * componentStructSize];

	//			uint8_t* targetAddress{ &componentAddress[distanceFromBegin * componentStructSize] };

	//			// copy mem to temporary place
	//			std::copy_n(
	//				targetAddress,
	//				componentStructSize,
	//				m_SwapResource.get()
	//			);

	//			// place the old to target
	//			std::copy_n(oldAddress,
	//				componentStructSize,
	//				targetAddress);

	//			index = distanceFromBegin;
	//		}
	//	}


	//	for (const auto& id : gameObjectIds)
	//	{
	//		this->ApplyPack(id, strideOffsets, winHandle, componentAddress, std::index_sequence_for<Args...>{});
	//		componentAddress += componentStructSize;
	//	}
	//}
}


#endif
