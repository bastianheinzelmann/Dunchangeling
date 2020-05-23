#pragma once

enum DungeonData{ DDE_Tile = 0, DDE_North = 1, DDE_East = 2, DDE_South, DDE_West = 3};

struct DungeonGrid
{
	DungeonGrid(int xSize, int ySize);

	int Get(int x, int y, DungeonData data);	
	void Set(int x, int y, DungeonData data, int value);

	int* DungeonArray;
	int XSize;
	int YSize;
private:
	int numData = 5;
};