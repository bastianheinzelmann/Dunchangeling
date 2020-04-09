#include <iostream>
#include "GeneticAlgorithm.h"
#include <assert.h>
#include <algorithm>

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

GeneticAlgorithm::GeneticAlgorithm(unsigned int popSize, unsigned int maxGens)
{
	populationSize = popSize;
	this->maxGenerations = maxGens;
	PopBuffer1 = std::vector<Graph>(popSize);
	PopBuffer2 = std::vector<Graph>(popSize);
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
		PopBuffer1[i] = graph_generateRandomGraphWilson(verticesNum, randomNumber(edgesNum, edgesNum + edgesTolerance), *this);
	}

	CurrentPopBuffer = &PopBuffer1;
}

void GeneticAlgorithm::currentGenerationToFile(const char * directory)
{
	int j = 0;
	for (auto i : *CurrentPopBuffer)
	{
		std::string file = std::string(directory).append("/gen").append(std::to_string(currentGeneration)).append("/ind").append(std::to_string(j)).append(".dt");
		//std::cout << file << std::endl;
		i.writeToFile(file.c_str());
		j++;
	}
}

void GeneticAlgorithm::nextGeneration()
{

}

void GeneticAlgorithm::run()
{
	while (currentGeneration < maxGenerations)
	{
		sort(PopBuffer1.begin(), PopBuffer1.end());

		int elitismRatio = (elitismRate * populationSize) / 100;
		for (int i = 0; i < elitismRatio; i++)
		{
			if (currentGeneration & 1 == 1)
			{
				// odd number
				PopBuffer1[i] = PopBuffer2[i];
			}
			else
			{
				// even number
				PopBuffer2[i] = PopBuffer1[i];
			}
		}

		int crossoverRatio = (crossoverRate * populationSize) / 100;
		for (int i = 0; i < crossoverRatio; i++)
		{
			// Tournament selection and mating
			// random(0, k best chromos)
		}



		for (int i = 0; i < populationSize; i++)
		{
			// mutation
		}


	}
}
