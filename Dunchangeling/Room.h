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
	int GetRoomArea();

	Room() {};

	// important for looking up in array
	int RoomID;

	Grid RoomGrid;
	std::vector<Grid> ConfigGrids;
private:
	int RoomArea = -1;
};

