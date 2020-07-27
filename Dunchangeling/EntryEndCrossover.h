#pragma once

#include "IGAFunctions.h"

struct EntryEndCrossover : public IGAFunctions
{
	DLLExport virtual Graph Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga);
	DLLExport virtual void Mutate(Graph & graph, GeneticAlgorithm & ga);
	DLLExport virtual void CalculateFitness(Graph & graph, GeneticAlgorithm & ga);

	DLLExport void SplitGraph(Graph graph, int cutPosition, int sourceIndex, Graph & splittedGraph, GeneticAlgorithm & ga);
	DLLExport float CalculateFlankingFitness(std::vector<unsigned int> & path1, std::vector<unsigned int> & path2);

	int pRemoveEdge = 20;
	int pAddEdge = 20;
	int pSwapEnd = 10;
	int pSwapEntry = 10;
	int pAddTreasure = 20;
	int pRemoveTreasure = 20;
};