#ifndef SLDFRAMEWORK_UTILS_H
#define SLDFRAMEWORK_UTILS_H

#include <type_traits>
#include "../Core/CustomTypes.h"
#include "../GameObject/GameObject.h"
#include "../Components/TransformComponent.h"
#include <box2d/box2d.h>

namespace rtm {
	struct float3f;
}

namespace SLD
{
	class Window;
	class TransformComponent;
	class Prefab;
	template<typename T,
		typename = std::enable_if_t<std::is_integral_v<T>>>
		constexpr const T& clamp(const T& val, const T& lo, const T& hi)
	{
		if (val > hi)
			return hi;
		if (val < lo)
			return  lo;

		return val;
	}

	inline void AllocateAndMove(WorldEntity& world, const SharedPtr<Archetype>& archetype)
	{
		const int occupiedSize{ archetype->sizeOfStruct * int(archetype->gameObjectIds.size()) };
		const int totalSize{ archetype->totalLength };


		if (!archetype->componentData)
		{
			const int newAllocateSize{ archetype->sizeOfStruct * 3 };

			SharedPtr<uint8_t[]> componentArray{
					SharedPtr<uint8_t[]>{new uint8_t[newAllocateSize]{}} };

			archetype->componentData = componentArray;
			archetype->totalLength = newAllocateSize;
			return;
		}
		if (occupiedSize + archetype->sizeOfStruct > totalSize)
		{
			const int newAllocateSize{ (occupiedSize + archetype->sizeOfStruct) * 3 };

			SharedPtr<uint8_t[]> componentArray{
			SharedPtr<uint8_t[]>{new uint8_t[newAllocateSize]{}} };

			uint8_t* oldDataAddress{ archetype->componentData.get() };
			uint8_t* newDataAddress{ componentArray.get() };

			archetype->totalLength = newAllocateSize;

			size_t offset{};
			for ([[maybe_unused]] auto&& __ : archetype->gameObjectIds)
			{
				for (const auto& typeId : archetype->types)
				{
					world.GetRegisterComponent(typeId)->MoveData(oldDataAddress + offset, newDataAddress + offset);
					offset += world.GetComponentSizeById(typeId);
				}
			}

			archetype->componentData = componentArray;
		}
	}
	
	template<typename T>
	EnableIsBasedOf<Prefab, T, GameObjectId> InstantiatePrefab(
		WorldEntity& world,
		T&& prefab,
		const rtm::float3f& worldPos)
	{
		T* ptr{ new T{std::move(prefab)} };

		SharedPtr<T> smart{ ptr };

		SharedPtr<GameObject> go{ GameObject::Create(world) };
		go->GetComponent<TransformComponent>()->Translate(worldPos);

		smart->OnCreate(go);
		
		return go->GetId();
	}

	template<typename T>
	EnableIsBasedOf<Prefab, T, GameObjectId> InstantiatePrefab(
		WorldEntity& world,
		T&& prefab,
		const rtm::float3f& worldPos,
		SharedPtr<T>& out)
	{
		T* ptr{ new T{std::move(prefab)} };

		SharedPtr<T> smart{ ptr };

		SharedPtr<GameObject> go{ GameObject::Create(world) };
		go->GetComponent<TransformComponent>()->Translate(worldPos);

		smart->OnCreate(go);
		out = smart;

		return go->GetId();
	}
}


inline b2PolygonShape CreateBox(float width, float height)
{
	b2PolygonShape shape{};
	shape.SetAsBox(height, width);
	return shape;
}

inline b2BodyDef CreateBasicBody(size_t gameObjectId, b2BodyType bodyType)
{
	b2BodyDef def{};
	def.type = bodyType;
	def.userData.pointer = gameObjectId;
	return def;
}

#endif

