#ifndef SLDFRAMEWORK_ECSTYPES_H
#define SLDFRAMEWORK_ECSTYPES_H

#include "../Core/Base.h"

namespace SLD
{
	using GameObjectId = size_t;
	using ComponentTypeId = size_t;

	struct ComponentStruct
	{
		OwnedPtr<uint8_t> elements{};
		//OwnedPtr<uint8_t[]> elements{};
		int size{};
	};

	// SoA structure
	struct Archetype
	{
		Archetype() = default;

		SharedPtr<Archetype> Clone()
		{
			SharedPtr<Archetype> out{ std::make_shared<Archetype>() };

			out->types = types;
			out->gameObjectIds = gameObjectIds;
			out->totalLength = totalLength;
			out->sizeOfStruct = sizeOfStruct;
			out->componentData = SharedPtr<uint8_t[]>{ new uint8_t[totalLength]{} };
			//out->numOfGameObjects = numOfGameObjects;

			if(!gameObjectIds.empty())
			{
				std::copy_n(
					componentData.get(),
					sizeOfStruct * int(gameObjectIds.size()),
					out->componentData.get());
			}

			return out;
		}

		std::vector<ComponentTypeId> types; // types of components of this archetypes
		std::vector<GameObjectId> gameObjectIds;
		SharedPtr<uint8_t[]> componentData{}; // Array of component struct
		int sizeOfStruct{};
		int totalLength{};
	};

	struct Record
	{
		WeakPtr<Archetype> restedArchetype{};
		int componentStructIdx{ -1 };
	};
}

#endif
