
#ifndef SLDFRAMEWORK_SYSTEMTEMPLATE_H
#define SLDFRAMEWORK_SYSTEMTEMPLATE_H

#include "SystemBase.h"
#include "../Core/WorldEntity.h"

namespace SLD
{
	template<typename ...ComponentTypes>
	class SystemTemplate : public SystemBase
	{
	public:

		SystemTemplate(WorldEntity& worldEntity, PipelineLayer layer = PipelineLayer::OnPreAsync);
		virtual ~SystemTemplate() override = default;

		virtual void OnUpdate(GameObjectId gameObjectId, float deltaTime, ComponentTypes*...) = 0;

	protected:

		virtual void InternalUpdate(float deltaTime, const SharedPtr<Archetype>& archetype) override;
		virtual void InternalRender(const SharedPtr<Window>&, const SharedPtr<Archetype>&) override final {}

		template<size_t Idx, size_t N>
		EnableIf<Idx != sizeof...(ComponentTypes)> DoTypeCheck(
			const std::vector<GameObjectId>& gameObjectsIds,
			const std::vector<ComponentTypeId>& archetypeIds,
			std::array<size_t, N>& strideOffsets,
			float deltaTime,
			uint8_t* componentAddress,
			int componentStructSize);

		template<size_t Idx, size_t N>
		EnableIf<Idx == sizeof...(ComponentTypes)> DoTypeCheck(
			const std::vector<GameObjectId>& gameObjectsIds,
			const std::vector<ComponentTypeId>& archetypeIds,
			std::array<size_t, N>& strideOffsets,
			float deltaTime,
			uint8_t* componentAddress,
			int componentStructSize);

		std::string SortKeys(std::vector<size_t>& keys);

		template<size_t... Idx, size_t N>
		void ApplyPack(
			GameObjectId id,
			float deltaTime,
			const std::array<size_t, N>& strideOffsets,
			uint8_t* componentAddress,
			std::index_sequence<Idx...>);

	};

	template <typename ... ComponentTypes>
	SystemTemplate<ComponentTypes...>::SystemTemplate(WorldEntity& worldEntity, PipelineLayer layer)
		: SystemBase(worldEntity, layer)
	{
		// sorted keys
		std::vector<size_t> keys{ {ComponentT<ComponentTypes>::GetId()...} };
		m_Key = SortKeys(keys);
	}

	template <typename ... ComponentTypes>
	void SystemTemplate<ComponentTypes...>::InternalUpdate(float deltaTime, const SharedPtr<Archetype>& archetype)
	{
		std::array<size_t, sizeof...(ComponentTypes)> offsets{};

		DoTypeCheck<0>(
			archetype->gameObjectIds,
			archetype->types,
			offsets,
			deltaTime,
			archetype->componentData.get(),
			archetype->sizeOfStruct
			);
	}

	template <typename ... ComponentTypes>
	template <size_t Idx, size_t N>
	EnableIf<Idx != sizeof...(ComponentTypes)> SystemTemplate<ComponentTypes...>::DoTypeCheck(
		const std::vector<GameObjectId>& gameObjectsIds,
		const std::vector<ComponentTypeId>& archetypeIds,
		std::array<size_t, N>& strideOffsets,
		float deltaTime, uint8_t* componentAddress,
		int componentStructSize)
	{
		size_t offsetFromStart{};

		using ComponentTypeFromSystem = std::tuple_element_t<Idx, std::tuple<ComponentTypes...>>;

		//const size_t thisStackComponentId{ BaseComponent::GetId<ComponentTypeFromSystem>() };
		const size_t thisStackComponentId{ ComponentT<ComponentTypeFromSystem>::GetId() };

		for (const auto& id : archetypeIds)
		{
			if (thisStackComponentId == id)
				break;

			offsetFromStart += this->m_World.GetComponentSizeById(id);
		}

		strideOffsets[Idx] = offsetFromStart;

		// Do recursion until we reached the last index of the parameter pack
		DoTypeCheck<Idx + 1>(
			gameObjectsIds,
			archetypeIds,
			strideOffsets,
			deltaTime,
			componentAddress,
			componentStructSize
			);
	}

	template <typename ... ComponentTypes>
	template <size_t Idx, size_t N>
	EnableIf<Idx == sizeof...(ComponentTypes)> SystemTemplate<ComponentTypes...>::DoTypeCheck(
		[[maybe_unused]] const std::vector<GameObjectId>& gameObjectsIds,
		[[maybe_unused]] const std::vector<ComponentTypeId>& archetypeIds,
		std::array<size_t, N>& strideOffsets,
		float deltaTime, uint8_t* componentAddress,
		int componentStructSize)
	{
		for (const auto& id : gameObjectsIds)
		{
			ApplyPack(id, deltaTime, strideOffsets, componentAddress, std::index_sequence_for<ComponentTypes...>());
			componentAddress += componentStructSize;
		}
	}

	template <typename ... ComponentTypes>
	std::string SystemTemplate<ComponentTypes...>::SortKeys(std::vector<size_t>& keys)
	{
		std::sort(keys.begin(), keys.end());

		std::stringstream out{};

		std::for_each(keys.begin(), keys.end(), [&out](size_t id)
			{
				out << id << '-';
			});

		return out.str();
	}

	template <typename ... ComponentTypes>
	template <size_t... Idx, size_t N>
	void SystemTemplate<ComponentTypes...>::ApplyPack(GameObjectId id, float deltaTime,
		const std::array<size_t, N>& strideOffsets, uint8_t* componentAddress, std::index_sequence<Idx...>)
	{
		OnUpdate(id, deltaTime, (reinterpret_cast<ComponentTypes*>(componentAddress + strideOffsets[Idx]))...);
	}
}



#endif