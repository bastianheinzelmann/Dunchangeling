#pragma once

struct GeneticAlgorithmProperties
{
	unsigned int populationSize = 100;
	unsigned int maxGenerations = 200;
	unsigned int elitismRate = 20;
	unsigned int crossoverRate = 0;
	unsigned int convergenceBorder = 10;
	bool doCrossover = true;
};