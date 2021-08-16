#pragma once

#include "../../Core/Base.h"

namespace SLD
{
	struct Archetype;

	struct RenderObject
	{
		SharedPtr<Archetype> parent{};
		size_t gameId{};
		int indexInStruct{ -1 };
	};

}
