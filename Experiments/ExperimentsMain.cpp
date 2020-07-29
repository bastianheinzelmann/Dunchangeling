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

using namespace GeneticAlgorithmUtils;

double getMedian(std::list<double> & list)
{
	if (list.size() % 2 == 0)
	{
		auto it = list.begin();
		std::advance(it, list.size() / 2);
		return (*it + *--it) / 2;
	}
	else
	{
		auto it = list.begin();
		std::advance(it, list.size() / 2);
		return *it;
	}
}

std::list<double> getSublist(int positionStart, int positionEnd, std::list<double> inputList)
{
	std::list<double> sublist;
	auto it1 = inputList.begin();
	auto it2 = inputList.begin();
	std::advance(it1, positionStart);
	std::advance(it2, positionEnd);

	for (; it1 != it2; it1++)
	{
		sublist.push_back(*it1);
	}

	return sublist;
}

std::list<double> removeOutliers(std::list<double> list)
{
	list.sort();
	std::list<double> data2; // big values
	std::list<double> data1; // small values

	std::list<double> cleanedList;

	if (list.size() % 2 == 0)
	{
		//auto it = list.begin();
		//std::advance(it, list.size() / 2);
		//list.splice(data2.begin(), data2, it, list.end());
		//data1 = list;

		data1 = getSublist(0, list.size() / 2, list);
		data2 = getSublist(list.size() / 2, list.size(), list);
	}
	else
	{
		//auto it = list.begin();
		//std::advance(it, (list.size() / 2) + 1);
		//list.splice(data2.begin(), data2, it, list.end());
		//data1 = list;
		data1 = getSublist(0, list.size() / 2 + 1, list);
		data2 = getSublist(list.size() / 2 + 1, list.size(), list);
	}
	double q1 = getMedian(data1);
	double q3 = getMedian(data2);
	double iqr = q3 - q1;
	double lowerFence = q1 - 1.5 * iqr;
	double upperFence = q3 + 1.5 * iqr;
	
	for (auto i : list)
	{
		if(i <= upperFence && i >= lowerFence)
		{
			cleanedList.push_back(i);
		}
	}

	return cleanedList;
}

void RunExperiments(int samples, int maxVertices, IGAFunctions* gaFunction, InitMode imode, bool flanks, bool crossover)
{
	using namespace std::chrono;

	DungeonProperties props;
	props.NumRooms = 10;
	props.NumSpecialRooms = 0;
	props.FlankingRoutes = flanks;
	props.branchingFactor = 0.f;
	props.OpponentTypes.emplace(1, OpponentInfo(1, 1));
	props.OpponentTypes.emplace(2, OpponentInfo(2, 2));
	props.OpponentTypes.emplace(3, OpponentInfo(3, 3));

	GeneticAlgorithmProperties gaProps;
	gaProps.convergenceBorder = 20;
	gaProps.elitismRate = 1;
	gaProps.doCrossover = crossover;

	high_resolution_clock::time_point t0;
	high_resolution_clock::time_point t1;

	double averageFitness;
	double averageTime;
	double averageGenerations;

	std::list<std::pair<int, double>> timeCoords;
	std::list<std::pair<int, double>> fitnessCoors;
	std::list<std::pair<int, double>> generationsCoord;
	std::list<std::pair<int, double>> varianceCoord;

	for (int vertices = 10; vertices <= maxVertices; vertices += 5)
	{
		std::cout << vertices << " Rooms \n";
		props.NumRooms = vertices;
		props.NumSpecialRooms += 1;
		std::list<double> timeList;
		std::list<double> fitnessList;
		std::list<double> generationsList;
		std::list<double> standardVarianceList;

		for (int i = 0; i < samples; i++)
		{
			GeneticAlgorithm ga = GeneticAlgorithm(gaProps, gaFunction, props);
			t0 = high_resolution_clock::now();
			ga.generateInitialPopulation(imode);
			ga.run();
			t1 = high_resolution_clock::now();
			std::cout << "Best fitness: " << (*ga.CurrentPopBuffer)[0].fitness << std::endl;

			fitnessList.push_back((*ga.CurrentPopBuffer)[0].fitness);
			timeList.push_back(duration_cast<duration<double>>(t1 - t0).count());
			generationsList.push_back(ga.currentGeneration);
		}

		std::list<double> cleanedFitness = removeOutliers(fitnessList);
		std::list<double> cleanedTime = removeOutliers(timeList);
		std::list<double> cleanedGens = removeOutliers(generationsList);

		// sample variance
		double averagegeneralFitness = std::accumulate(fitnessList.begin(), fitnessList.end(), 0.f) / fitnessList.size();
		double squared = 0;
		for (auto x : fitnessList)
		{
			squared += std::pow(x - averagegeneralFitness, 2);
		}
		double sampleVariance = squared / (fitnessList.size() - 1);
		double sampleStandardVariance = std::sqrt(sampleVariance);
		//---


		averageTime = std::accumulate(cleanedTime.begin(), cleanedTime.end(), 0.f) / cleanedTime.size();
		averageFitness = std::accumulate(cleanedFitness.begin(), cleanedFitness.end(), 0.f) / cleanedFitness.size();
		averageGenerations = std::accumulate(cleanedGens.begin(), cleanedGens.end(), 0.f) / cleanedGens.size();

		timeCoords.push_back(std::pair<int, double>(vertices, averageTime));
		fitnessCoors.push_back(std::pair<int, double>(vertices, averageFitness));
		generationsCoord.push_back(std::pair<int, double>(vertices, averageGenerations));
		varianceCoord.push_back(std::pair<int, double>(vertices, sampleStandardVariance));
	}

	std::cout << "Time: \n";

	for (auto time : timeCoords)
	{
		std::cout << "(" << time.first << ", " << time.second << ")";
	}

	std::cout << "Fitness: \n";

	for (auto fitness : fitnessCoors)
	{
		std::cout << "(" << fitness.first << ", " << fitness.second << ")";
	}

	std::cout << "Generations: \n";

	for (auto gen : generationsCoord)
	{
		std::cout << "(" << gen.first << ", " << gen.second << ")";
	}

	std::cout << "Standardvariance: \n";

	for (auto vari : varianceCoord)
	{
		std::cout << "(" << vari.first << ", " << vari.second << ")";
	}
}

void RunLayoutExperiments(int numrooms, int numSpecialRooms, IGAFunctions* gaFunction)
{
	using namespace std::chrono;

	DungeonProperties props;
	props.NumRooms = numrooms;
	props.NumSpecialRooms = numSpecialRooms;
	props.FlankingRoutes = true;
	props.branchingFactor = 0.f;
	props.OpponentTypes.emplace(1, OpponentInfo(1, 1));
	props.OpponentTypes.emplace(2, OpponentInfo(2, 2));
	props.OpponentTypes.emplace(3, OpponentInfo(3, 3));

	GeneticAlgorithmProperties gaProps;
	gaProps.convergenceBorder = 20;
	gaProps.elitismRate = 1;
	gaProps.doCrossover = true;

	high_resolution_clock::time_point t0;
	high_resolution_clock::time_point tGaDone;
	high_resolution_clock::time_point t1;

	GeneticAlgorithm ga = GeneticAlgorithm(gaProps, gaFunction, props);
	t0 = high_resolution_clock::now();
	ga.generateInitialPopulation(InitMode::EIM_PATH);
	ga.run();
	Graph gaGraph = (*ga.CurrentPopBuffer)[0];
	tGaDone = high_resolution_clock::now();
	double time = duration_cast<duration<double>>(tGaDone - t0).count();
	std::cout << "Ga is done: " << time << std::endl;

	BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(gaGraph);
	//GraphToDot(bg);
	Chains chains = GeneticAlgorithmUtils::ChainDecomposition(bg);
	//GeneticAlgorithmUtils::GraphChainsDot(bg, chains);

	//for (auto chain : chains)
	//{
	//	for (auto c : chain)
	//	{
	//		std::cout << c << " ";
	//	}
	//	std::cout << std::endl;
	//}

	unsigned int room[] =
	{
		0, 0, 1, 1,
		0, 0, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1
	};

	unsigned int room2[] =
	{
		1, 1,
		1, 1
	};

	unsigned int room3[] =
	{
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	unsigned int room4[] =
	{
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1
	};

	unsigned int room5[] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 0, 1, 1,
		0, 0, 1, 1
	};

	unsigned int room6[] = {
		1, 1, 0, 0,
		1, 1, 0, 0,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 0, 0,
		1, 1, 0, 0
	};

	unsigned int room7[] = {
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1
	};

	unsigned int room8[] = {
		1, 1, 1,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 1, 1,
	};

	//unsigned int room5[] = {
	//	1, 1, 1, 1,
	//	1, 0, 0, 1,
	//	1, 0, 0, 1,
	//	1, 1, 1, 1
	//};

	Grid grid(4, 4, room);
	Grid grid2(2, 2, room2);
	Grid grid3(3, 5, room3);
	Grid grid4(5, 3, room4);
	Grid grid5(4, 4, room5);
	Grid grid6(4, 6, room6);
	Grid grid7(1, 9, room7);
	Grid grid8(9, 1, room7);
	Grid grid9(3, 8, room8);

	std::vector<Room> rooms = { Room(grid), Room(grid2), Room(grid3), Room(grid4), Room(grid5), Room(grid6) };

	GraphToMap::RoomCollection roomCollection(rooms);

	GraphToMap::MapGenerator mg(roomCollection, chains, bg);
	std::vector<std::pair<Layout, std::string>> debugLayout;

	std::vector<Layout> layoutHistory;
	Layout finalLayout = mg.GenerateLayout(bg, layoutHistory);
	t1 = high_resolution_clock::now();
	time = duration_cast<duration<double>>(t1 - t0).count();
	std::cout << "Everything is done: " << time << std::endl;
}

int main()
{

	std::cout << "Run!\n";

	EntryEndCrossover * edgeBased = new EntryEndCrossover();
	ProductionRules * productions = new ProductionRules();

	// Edge based, Random Initialization, no flanks, no crossover
	//RunExperiments(20, 50, edgeBased, InitMode::EIM_RANDOM, false, false);
	// Edge based, Random Initialization, no flanks, crossover
	//RunExperiments(20, 50, edgeBased, InitMode::EIM_RANDOM, false, true);

	// edge based, random, flanks, crossover
	// RunExperiments(20, 50, edgeBased, InitMode::EIM_RANDOM, true, true);

	// Edge based, path graph Initialization, no flanks, crossover
	// RunExperiments(20, 50, edgeBased, InitMode::EIM_PATH, false, true);

	// Edge based, path graph Initialization, flanks, no crossover
	 //RunExperiments(20, 30, edgeBased, InitMode::EIM_PATH, true, false);
	// Edge based, path graph Initialization, flanks, crossover
	// RunExperiments(20, 50, edgeBased, InitMode::EIM_PATH, true, true);

	// production based, path graph Initialization, flanks, crossover
	// RunExperiments(20, 50, productions, InitMode::EIM_PATH_THREE, true, true);

	// production based, random, flanks, crossover
	// RunExperiments(20, 50, productions, InitMode::EIM_RANDOM, true, true);
	
	// production based, path graph Initialization, no flanks, no crossover
	// RunExperiments(20, 40, productions, InitMode::EIM_PATH_THREE, false, false);

	// production based, path graph Initialization, flanks, no crossover
	 //RunExperiments(20, 50, productions, InitMode::EIM_PATH_THREE, true, false);

	// production based, path graph Initialization, no flanks, crossover
	// RunExperiments(20, 50, productions, InitMode::EIM_PATH_THREE, false, true);

	for(int i = 0; i < 20; i++)
		RunLayoutExperiments(30, 5, edgeBased);

	std::cout << "Press key to terminate: ";
	std::getchar();

	return 0;
}