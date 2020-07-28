#pragma once

#include "Room.h"
#include <vector>
#include "Graph.h"

struct LayoutRoom
{
	Room Room;
	int VertexID;
	int PosX;
	int PosY;

	VertexAttributes Attributes;

	std::vector<int> Neighbours;
	std::vector<int> RoomsNotConnected;
};