#ifndef SLDFRAMEWORK_REALTIMEMATH_H
#define SLDFRAMEWORK_REALTIMEMATH_H

#include "rtm/math.h"
#include "rtm/vector4f.h"

namespace SLD
{
	template<typename T>
	struct Vector2
	{
		Vector2() = default;

		T x{};
		T y{};
	};

	template<typename T>
	struct Vector4
	{
		Vector4() = default;


		T x{};
		T y{};
		T z{};
		T a{};
	};
}


#endif