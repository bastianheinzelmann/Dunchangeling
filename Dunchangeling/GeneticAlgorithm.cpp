#include <iostream>
#include "GeneticAlgorithm.h"
#include <assert.h>

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
