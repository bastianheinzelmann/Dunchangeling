#include "DungeonGrid.h"
#include <assert.h>

DungeonGrid::DungeonGrid(int xSize, int ySize)
{
	DungeonArray = new int[xSize * ySize * numData];

	XSize = xSize;
	YSize = ySize;

	for (int i = 0; i < xSize * ySize * numData; i++)
	{
		DungeonArray[i] = 0;
	}
}

int DungeonGrid::Get(int x, int y, DungeonData data)
{
	assert(x >= 0 && x < XSize && y >= 0 && y < YSize);
	return DungeonArray[(XSize * numData * y) + (x * numData) + data];
}

void DungeonGrid::Set(int x, int y, DungeonData data, int value)
{
	assert(x >= 0 && x < XSize && y >= 0 && y < YSize);
	DungeonArray[(XSize * numData * y) + (x * numData) + data] = value;
}