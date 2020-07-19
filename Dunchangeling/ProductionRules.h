#pragma once

#include "IGAFunctions.h"

struct ProductionRules : public IGAFunctions
{
	DLLExport virtual Graph Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga);
	DLLExport virtual void Mutate(Graph & graph, GeneticAlgorithm & ga);
	DLLExport virtual void CalculateFitness(Graph & graph, GeneticAlgorithm & ga);

	DLLExport float CalculateFlankingFitness(std::vector<unsigned int> & path1, std::vector<unsigned int> & path2);
	void SplitGraph(Graph graph, int cutPosition, int sourceIndex, Graph & splittedGraph, GeneticAlgorithm & ga);

	int pAddVertex1 = 15;
	int pAddVertex2 = 15;
	int pRemoveVertex = 50;
	int pAddCycle = 10;
	int pAddTreasure = 10;

	//int pAddVertex1 = 13;
	//int pAddVertex2 = 13;
	//int pRemoveVertex = 44;
	//int pAddCycle = 9;
	//int pAddTreasure = 9;
	int pAddOpponent = 0;
	int pRemoveOpponent = 0;
};