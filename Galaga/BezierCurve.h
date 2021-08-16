#pragma once

#include <SLDFramework.h>

struct BezierCurve
{
	rtm::float2f startPoint;
	rtm::float2f controlStart;
	rtm::float2f controlEnd;
	rtm::float2f endPoint;

	rtm::float2f CalculateCurvePoint(float t) const
	{
		const float tSqr{ t * t };
		const float tCube{ tSqr * t };

		const float u{ 1.0f - t };
		const float uSqr{ u * u };
		const float uCube{ uSqr * u };

		const auto startVec{ rtm::vector_load2(&startPoint) };
		const auto controlStartVec{ rtm::vector_load2(&controlStart) };
		const auto controlEndVec{ rtm::vector_load2(&controlEnd) };
		const auto endVec{ rtm::vector_load2(&endPoint) };

		const auto v1 = rtm::vector_mul(startVec, uCube);
		const auto v2 = rtm::vector_mul(controlStartVec, 3 * uCube * t);
		const auto v3 = rtm::vector_mul(controlEndVec, 3 * u * tSqr);
		const auto v4 = rtm::vector_mul(endVec, tCube);

		const auto result = rtm::vector_add(rtm::vector_add(v1, v2),rtm::vector_add(v3,v4));

		rtm::float2f calPoint{};
		rtm::vector_store2(result, &calPoint);

		return calPoint;
	}
};
