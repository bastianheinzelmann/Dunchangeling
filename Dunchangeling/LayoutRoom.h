#pragma once

#include "Room.h"
#include <vector>

struct LayoutRoom
{
	Room Room;
	int VertexID;
	int PosX;
	int PosY;

	std::vector<int> Neighbours;
};