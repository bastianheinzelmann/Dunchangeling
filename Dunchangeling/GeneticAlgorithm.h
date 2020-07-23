#pragma once
#include <vector>
#include "Graph.h"
#include "RNG.h"
#include <random>
#include <iostream>
#include <assert.h>
#include "Constants.h"
#include <chrono>
#include <math.h>
#include "IGAFunctions.h"
#include "DungeonProperties.h"
#include "GeneticAlgorithmProperties.h"

enum InitMode { EIM_PATH_THREE = 0, EIM_PATH = 1, EIM_RANDOM = 2};

class GeneticAlgorithm
{
public:
	GeneticAlgorithm() {};
	DLLExport GeneticAlgorithm(GeneticAlgorithmProperties gaProps, IGAFunctions * functions, DungeonProperties props);

	std::vector<Graph>* CurrentPopBuffer;

	std::vector<Graph> PopBuffer1;
	std::vector<Graph> PopBuffer2;

	DLLExport PopId requestId();
	DLLExport VertexName requestVertexName();

	DLLExport void generateInitialPopulation(InitMode initMode);
	DLLExport void currentGenerationToFile(const char* directory);

	DLLExport void run();	

	DungeonProperties DProperties;

private:
	
	Graph& TournamentSelection(int k);
	void calculateFitness();

	unsigned int populationSize = 0;
	unsigned int maxGenerations = 0;
	unsigned int elitismRate = 20;
	unsigned int crossoverRate = 0;
	bool doCrossover = true;

	float highestFitness;
	unsigned int nothingChangedCount = 0;
	unsigned int convergenceBorder = 10;

	const unsigned int generationBits = GENERATION_BITS;
	PopId currentPopId = 0;
	VertexName currentVertexName = 0;
	unsigned int currentGeneration = 0;

	IGAFunctions * gaFunctions;
};
