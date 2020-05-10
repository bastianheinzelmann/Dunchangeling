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

class GeneticAlgorithm
{
public:
	GeneticAlgorithm() {};
	DLLExport GeneticAlgorithm(unsigned int popSize, unsigned int maxGens);

	std::vector<Graph>* CurrentPopBuffer;

	std::vector<Graph> PopBuffer1;
	std::vector<Graph> PopBuffer2;

	DLLExport PopId requestId();
	DLLExport VertexName requestVertexName();

	DLLExport void generateInitialPopulation(unsigned int verticesNum, unsigned int edgesNum, unsigned int edgesTolerance);
	DLLExport void currentGenerationToFile(const char* directory);

	DLLExport void nextGeneration();
	DLLExport void run();

private:
	
	Graph& TournamentSelection(int k);
	void calculateFitness();

	unsigned int populationSize = 0;
	unsigned int maxGenerations = 0;
	unsigned int elitismRate = 10;
	unsigned int crossoverRate = 90;

	float highestFitness;
	unsigned int nothingChangedCount = 0;
	unsigned int convergenceBorder = 100;

	std::vector<Graph> BrokenPopulation;

	const unsigned int generationBits = GENERATION_BITS;
	PopId currentPopId = 0;
	VertexName currentVertexName = 0;
	unsigned int currentGeneration = 0;
};
