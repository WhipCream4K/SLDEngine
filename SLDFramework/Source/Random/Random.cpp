#include "Random.h"

std::default_random_engine SLD::Random::staticERand{ std::random_device()() };

int SLD::Random::RandInt(int from, int to)
{
	const std::uniform_int_distribution dt{ from,to };
	return dt(staticERand);
}

float SLD::Random::RandFloat(float from, float to)
{
	const std::uniform_real_distribution<float> dt{ from,to };
	return dt(staticERand);
}
