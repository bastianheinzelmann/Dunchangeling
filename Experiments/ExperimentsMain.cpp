#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>
#include <random>
#include "../Dunchangeling/GeneticAlgorithm.h" 
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"
#include "../Dunchangeling/GraphUtils.cpp"
#include "../Dunchangeling/GeneticAlgorithmUtils.h"
#include "../Dunchangeling/GraphToMap.h"
#include "../Dunchangeling/EntryEndCrossover.h"
#include "../Dunchangeling/ProductionRules.h"
#include <chrono>
#include <ctime>
#include <functional>
#include <numeric>

void RunExperiments(int samples)
{
	using namespace std::chrono;
	EntryEndCrossover * edgeBased = new EntryEndCrossover();
	ProductionRules * productions = new ProductionRules();

	DungeonProperties props;
	props.NumRooms = 10;
	props.NumSpecialRooms = 1;
	props.FlankingRoutes = false;
	props.branchingFactor = 0.f;

	GeneticAlgorithmProperties gaProps;
	gaProps.doCrossover = false;

	high_resolution_clock::time_point t0;
	high_resolution_clock::time_point t1;

	std::list<int> timeList;
	std::list<int> fitnessList;

	int averageFitness;
	double averageTime;

	// simple path mutation only edgebased
	for (int i = 0; i < samples; i++)
	{
		t0 = high_resolution_clock::now();
		GeneticAlgorithm ga = GeneticAlgorithm(gaProps, edgeBased, props);
		t1 = high_resolution_clock::now();

		fitnessList.push_back((*ga.CurrentPopBuffer)[0].fitness);
		timeList.push_back(duration_cast<duration<double>>(t1 - t0).count());
	}

	averageTime = std::accumulate(timeList.begin(), timeList.end(), 0) / samples;
	averageFitness = std::accumulate(fitnessList.begin(), fitnessList.end(), 0) / samples;

	std::cout << "Average time: " << averageTime << std::endl;
	std::cout << "Average fitness: " << averageTime << std::endl;
}

int main()
{
	RunExperiments(10);

	std::getchar();

	return 0;
}