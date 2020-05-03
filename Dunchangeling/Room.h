#pragma once

#include "Grid.h"
#include "Constants.h"
#include <iostream>
#include <vector>

class Room
{
public:
	DLLExport Room(Grid grid);

	DLLExport Grid CalculateConfigGrid(Room& otherRoom);

	Grid RoomGrid;
	std::vector<Grid> ConfigGrids;
};

