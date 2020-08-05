#ifndef TESTDLL_NATIVE_LIB_H
#define TESTDLL_NATIVE_LIB_H

#define DLLExport __declspec(dllexport)

#include "UDungeonProperties.h"
#include "UGeneticAlgorithmProperties.h"

const int EDGE_MUTATIONS = 0;
const int PRODUCTION_MUTATIONS = 1;
const int INIT_PATH = 0;
const int INIT_RANDOM = 1;

extern "C"
{
	DLLExport int generateLayout(int** data, int* xSize, int* ySize, int* numData, UDungeonProperties dprops, int concatenatedRoomArray[], UGeneticAlgorithmProperties gaProps);
}
#endif