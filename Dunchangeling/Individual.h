#pragma once
#include "Graph.h"

class Individual
{
public:
	unsigned int fitness = 0;
	Graph chromosome;
private:

};


bool operator<(const Individual &ind1, const Individual &ind2)
{
	return ind1.fitness < ind2.fitness;
}