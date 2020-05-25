#pragma once

#include "Constants.h"

enum DungeonData{ DDE_Tile = 0, DDE_North = 1, DDE_East = 2, DDE_South = 3, DDE_West = 4};

struct DungeonGrid
{
	DungeonGrid() { DungeonArray = nullptr;  };
	DungeonGrid(int xSize, int ySize);

	DLLExport int Get(int x, int y, DungeonData data);	
	DLLExport void Set(int x, int y, DungeonData data, int value);

	int* DungeonArray;
	int XSize;
	int YSize;
	int numData = 5;
};