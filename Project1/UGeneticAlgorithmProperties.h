#pragma once

extern "C" __declspec(dllexport) struct UGeneticAlgorithmProperties
{
	int populationSize;
	int maxGenerations;
	int elitismRate;
	int crossoverRate;
	int convergenceBorder;
	int mutationType;
	int initMode;
	bool doCrossover;
};