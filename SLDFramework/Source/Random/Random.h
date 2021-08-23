#pragma once

#include <random>

namespace SLD
{
	class Random
	{
	public:

		static int RandInt(int from, int to);
		static float RandFloat(float from, float to);

	private:

		static std::default_random_engine staticERand;
	};
}



