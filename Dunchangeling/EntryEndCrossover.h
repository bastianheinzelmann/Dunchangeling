#pragma once

#include "IGAFunctions.h"

class EntryEndCrossover : public IGAFunctions
{
	DLLExport virtual Graph Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga);
	DLLExport virtual void Mutate(Graph & graph, GeneticAlgorithm & ga);
	DLLExport virtual void CalculateFitness(Graph & graph, GeneticAlgorithm & ga);
};