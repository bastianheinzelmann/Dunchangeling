#include <iostream>
#include "GeneticAlgorithm.h"
#include <assert.h>

using namespace GraphUtils;

PopId GeneticAlgorithm::requestId()
{
	unsigned int currentId = currentPopId & (1 << generationBits - 1);
	currentId += 1;

	if (currentId > (1 << ID_BITS) - 1)
	{
		// TODO problem all ids are given
		std::cout << "All ids are given" << std::endl;
	}

	assert(currentId < (1 << ID_BITS - 1));

	return ++currentPopId;
}

GeneticAlgorithm::GeneticAlgorithm(unsigned int popSize)
{
	populationSize = popSize;
	Population = std::vector<Graph>(popSize);
	BrokenPopulation = std::vector<Graph>(popSize);
}

void GeneticAlgorithm::generateInitialPopulation(unsigned int verticesNum, unsigned int edgesNum, unsigned int edgesTolerance)
{
	if (populationSize <= 0)
	{
		std::cout << "Initialize Population size bigger than 0!" << std::endl;
	}

	for (int i = 0; i < populationSize; i++)
	{
		Population[i] = graph_generateRandomGraphWilson(verticesNum, randomNumber(edgesNum, edgesNum + edgesTolerance), *this);
	}
}

void GeneticAlgorithm::currentGenerationToFile(const char * directory)
{
	int j = 0;
	for (auto i : Population)
	{
		std::string file = std::string(directory).append("/gen").append(std::to_string(currentGeneration)).append("/ind").append(std::to_string(j)).append(".dt");
		//std::cout << file << std::endl;
		i.writeToFile(file.c_str());
		j++;
	}
}
