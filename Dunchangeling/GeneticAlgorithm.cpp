#include <iostream>
#include "GeneticAlgorithm.h"
#include <assert.h>
#include <algorithm>
#include <bitset>
#include "GeneticAlgorithmUtils.h"

PopId GeneticAlgorithm::requestId()
{
	unsigned int currentId = currentPopId & ((1 << ID_BITS) - 1);
	currentId += 1;

	if (currentId > (1 << ID_BITS) - 1)
	{
		// TODO problem all ids are given
		std::cout << "All ids are given" << std::endl;
	}

	assert(currentId < (1 << ID_BITS - 1));

	PopId generationPart = currentGeneration << ID_BITS;
	currentId += generationPart;

	currentPopId = currentId;

	//std::bitset<32> bits(currentId);
	//std::cout << "As binary: " << (bits) << std::endl;
	//std::cout << "Current popid: " << (currentPopId & (1u << ID_BITS) - 1) << std::endl;

	return currentPopId;
	//return ++currentPopId;
}

VertexName GeneticAlgorithm::requestVertexName()
{
	if (currentVertexName == UINT_MAX)
	{
		std::cout << "Vertexnames are all given" << std::endl;
		assert(false);
	}

	return currentVertexName++;
}

GeneticAlgorithm::GeneticAlgorithm(unsigned int popSize, unsigned int maxGens, IGAFunctions * functions)
{
	gaFunctions = functions;
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
	calculateFitness();
	sort(PopBuffer1.begin(), PopBuffer1.end(), std::greater<Graph>());
	highestFitness = PopBuffer1[0].fitness;

	std::cout << "Initial Best fitness: " << PopBuffer1[0].fitness << std::endl;
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

Graph & GeneticAlgorithm::TournamentSelection(int k)
{
	std::vector<Graph> & popBuffer = (*CurrentPopBuffer);
	Graph& best = popBuffer[randomNumber(0, populationSize - 1)];

	for (int i = 1; i < k; i++)
	{
		Graph& graph = popBuffer[randomNumber(0, populationSize / 2)];
		if (best < graph)
		{
			best = graph;
		}
	}

	return best;
}

void GeneticAlgorithm::calculateFitness()
{
	for (int i = 0; i < CurrentPopBuffer->size(); i++)
	{
		(*CurrentPopBuffer)[i].calculateFitness();
	}
}

void GeneticAlgorithm::run()
{
	while (currentGeneration < maxGenerations && nothingChangedCount < convergenceBorder)
	{
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

		if (doCrossover)
		{
			int crossoverRatio = (crossoverRate * populationSize) / 100;
			for (int i = elitismRatio; i < populationSize; i++)
			{
				Graph & parent1 = TournamentSelection(3);
				Graph & parent2 = TournamentSelection(3);

				//Graph matedGraph = graph_crossover(parent1, parent2, *this);
				Graph matedGraph = gaFunctions->Crossover(parent1, parent2, *this);
				if (currentGeneration & 1 == 1)
				{
					// odd number
					PopBuffer1[i] = matedGraph;
				}
				else
				{
					// even number
					PopBuffer2[i] = matedGraph;
				}
			}
		}
		else
		{
			for (int i = elitismRatio; i < populationSize; i++)
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
		}

		if (currentGeneration == 55)
		{
			std::cout << "Hii" << std::endl;
		}

		for (int i = elitismRatio; i < populationSize; i++)
		{
			if (currentGeneration & 1 == 1)
			{
				// odd number
				//graph_mutate(PopBuffer1[i], *this);
				gaFunctions->Mutate(PopBuffer1[i], *this);
			}
			else
			{
				// even number
				//graph_mutate(PopBuffer2[i], *this);
				gaFunctions->Mutate(PopBuffer2[i], *this);
			}
		}
		
		if (currentGeneration & 1 == 1)
		{
			//sort(PopBuffer1.begin(), PopBuffer1.end(), std::greater<Graph>());
			CurrentPopBuffer = &PopBuffer1;
		}
		else
		{
			//even
			//sort(PopBuffer2.begin(), PopBuffer2.end(), std::greater<Graph>());
			CurrentPopBuffer = &PopBuffer2;
		}

		calculateFitness();
		sort(CurrentPopBuffer->begin(), CurrentPopBuffer->end(), std::greater<Graph>());

		if (highestFitness < (*CurrentPopBuffer)[0].fitness)
		{
			highestFitness = (*CurrentPopBuffer)[0].fitness;
			nothingChangedCount = 0;
		}
		else
		{
			++nothingChangedCount;
		}

		currentGeneration++;
		currentPopId = 0;

		std::cout << "Current best fitness: " << (*CurrentPopBuffer)[0].fitness << " Gen: " << currentGeneration << std::endl;
		//currentGenerationToFile("C:/Users/Bastian/Documents/MasterStuff/Test");
	}
}
