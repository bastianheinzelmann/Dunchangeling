#pragma once
#include <random>

class RNG
{
private:
	static std::random_device dev;
	static std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> distribution;
public:
	RNG(int min, int max)
	{
		distribution = std::uniform_int_distribution<std::mt19937::result_type>(min, max);
	}

	static void init()
	{
		rng = std::mt19937(dev());
	}

	int generate() { return distribution(rng); };
};