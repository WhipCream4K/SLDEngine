
#ifndef SLD_FRAMEWORK_RENDERSYSTEM
#define SLD_FRAMEWORK_RENDERSYSTEM

#include "../../Core/Base.h"
#include "../../System/SystemBase.h"
#include "../../Components/TransformComponent.h"

namespace SLD
{
	template<typename ...ComponentTypes>
	class RenderSystem : public SystemBase
	{
	public:

		RenderSystem(WorldEntity& world);

		virtual void InternalUpdate(float, const SharedPtr<Archetype>&) override final {}
		virtual void InternalRender(const SharedPtr<Window>& winHandle, const SharedPtr<Archetype>& archetype) override final;

		virtual void OnRender(const SharedPtr<Window>& renderWindow, GameObjectId id, const TransformComponent*, ComponentTypes*...) = 0;

		virtual ~RenderSystem() override = default;

	protected:

		virtual void CallSystemRenderLoop(
			std::vector<ComponentTypeId>& gameObjectIds,
			const SharedPtr<Window>& winHandle,
			const std::vector<size_t>& strideOffsets,
			uint8_t* componentAddress,
			int componentStructSize);

		template<size_t... Idx>
		void ApplyPack(
			GameObjectId id,
			const std::vector<size_t>& strideOffsets,
			const SharedPtr<Window>& winHandle,
			uint8_t* componentAddress,
			std::index_sequence<Idx...>);

		std::vector<size_t> m_StrideOffsets;

	protected:

		template<size_t Idx>
		EnableIf<Idx != sizeof...(ComponentTypes) + 1> DoTypeCheck(
			std::vector<GameObjectId>& gameObjectsIds,
			const std::vector<ComponentTypeId>& archetypeIds,
			std::vector<size_t>& strideOffsets,
			const SharedPtr<Window>& winHandle,
			uint8_t* componentAddress,
			int componentStructSize);

		template<size_t Idx>
		EnableIf<Idx == sizeof...(ComponentTypes) + 1> DoTypeCheck(
			std::vector<GameObjectId>& gameObjectsIds,
			const std::vector<ComponentTypeId>& archetypeIds,
			std::vector<size_t>& strideOffsets,
			const SharedPtr<Window>& winHandle,
			uint8_t* componentAddress,
			int componentStructSize);

		std::string SortKeys(std::vector<size_t>& keys);

	};

	template <typename ... ComponentTypes>
	RenderSystem<ComponentTypes...>::RenderSystem(WorldEntity& world)
		: SystemBase(world, PipelineLayer::OnRender)
		, m_StrideOffsets()
	{
		std::vector<size_t> keys{ {ComponentT<TransformComponent>::GetId(),ComponentT<ComponentTypes>::GetId()...} };
		m_Key = SortKeys(keys);
		m_StrideOffsets.resize(sizeof...(ComponentTypes) + 1);
		//m_StrideOffsets.reserve(sizeof...(ComponentTypes));
	}

	template <typename ... ComponentTypes>
	void RenderSystem<ComponentTypes...>::InternalRender(const SharedPtr<Window>& winHandle,
		const SharedPtr<Archetype>& archetype)
	{
		DoTypeCheck<0>(
			archetype->gameObjectIds,
			archetype->types,
			m_StrideOffsets,
			winHandle,
			archetype->componentData.get(),
			archetype->sizeOfStruct
			);
	}

	template <typename ... ComponentTypes>
	template <size_t Idx>
	EnableIf<Idx != sizeof...(ComponentTypes) + 1> RenderSystem<ComponentTypes...>::DoTypeCheck(
		std::vector<GameObjectId>& gameObjectsIds, 
		const std::vector<ComponentTypeId>& archetypeIds,
		std::vector<size_t>& strideOffsets,
		const SharedPtr<Window>& winHandle, uint8_t* componentAddress, int componentStructSize)
	{
		size_t offsetFromStart{};

		using ComponentTypeFromSystem = std::tuple_element_t<Idx, std::tuple<TransformComponent, ComponentTypes...>>;

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
			winHandle,
			componentAddress,
			componentStructSize
			);
	}


	template <typename ... ComponentTypes>
	template <size_t Idx>
	EnableIf<Idx == sizeof...(ComponentTypes) + 1> RenderSystem<ComponentTypes...>::DoTypeCheck(
		std::vector<GameObjectId>& gameObjectsIds,
		[[maybe_unused]] const std::vector<ComponentTypeId>& archetypeIds,
		std::vector<size_t>& strideOffsets,
		const SharedPtr<Window>& winHandle,
		uint8_t* componentAddress,
		int componentStructSize)
	{
		CallSystemRenderLoop(gameObjectsIds, winHandle, strideOffsets, componentAddress, componentStructSize);
	}

	template <typename ... ComponentTypes>
	std::string RenderSystem<ComponentTypes...>::SortKeys(std::vector<size_t>& keys)
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
	void RenderSystem<ComponentTypes...>::CallSystemRenderLoop(
		std::vector<ComponentTypeId>& gameObjectIds,
		const SharedPtr<Window>& winHandle, const std::vector<size_t>& strideOffsets, uint8_t* componentAddress,
		int componentStructSize)
	{
		for (const auto& id : gameObjectIds)
		{
			ApplyPack(id, strideOffsets, winHandle, componentAddress, std::index_sequence_for<ComponentTypes...>{});
			componentAddress += componentStructSize;
		}
	}

	template <typename ... ComponentTypes>
	template <size_t... Idx>
	void RenderSystem<ComponentTypes...>::ApplyPack(GameObjectId id, const std::vector<size_t>& strideOffsets,
		const SharedPtr<Window>& winHandle, uint8_t* componentAddress, std::index_sequence<Idx...>)
	{
		OnRender(
			winHandle,
			id,
			reinterpret_cast<const TransformComponent*>(componentAddress + strideOffsets[0]),
			// count from 1 because 0 is TransformComponent
			reinterpret_cast<ComponentTypes*>(componentAddress + strideOffsets[Idx + 1])...);
	}
}


#endif
