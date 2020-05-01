#pragma once

#include "Grid.h"
#include "Constants.h"
#include <iostream>

class Room
{
public:
	DLLExport Room(Grid grid);

	void CalculateConfigGrid(Room& otherRoom);
	Grid CalculateHull();

	Grid RoomGrid;
	Grid RoomHull;
	Grid* ConfigGrids;
};

