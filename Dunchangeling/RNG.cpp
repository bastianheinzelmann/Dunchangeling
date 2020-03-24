#include "RNG.h"
#include <random>

void RNG::setDistribution(int min, int max)
{
	distribution = std::uniform_int_distribution<std::mt19937::result_type>(min, max);
}