#pragma once
#include "Graph.h"

class GeneticAlgorithm;

struct IGAFunctions
{
	virtual Graph Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga) = 0;
	virtual void Mutate(Graph & graph, GeneticAlgorithm & ga) = 0;

	virtual ~IGAFunctions() {}
};