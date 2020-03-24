#pragma once
#include <random>

#define DLLExport __declspec(dllexport)

class RNG
{
public:
	DLLExport static RNG& getInstance()
	{
		static RNG instance;
		return instance;
	}

	DLLExport void setDistribution(int min, int max);
	DLLExport int operator() ()
	{
		return distribution(engine);
	}

private:
	RNG() 
	{
		engine = std::mt19937(dev());
	}

	std::random_device dev;
	std::mt19937 engine;
	std::uniform_int_distribution<std::mt19937::result_type> distribution;

public:
	RNG(RNG const&) = delete;
	void operator=(RNG const&) = delete;
};