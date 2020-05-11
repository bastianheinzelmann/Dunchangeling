#pragma once

#include <vector>
#include "LayoutRoom.h"

class Layout
{
public:
	Layout(int NumRooms);

	// name of vertex is index of vector!
	std::vector<LayoutRoom> Rooms;
	std::vector<bool> LaidOutVertices;
	float GetEnergy();
	int GetIntersectionArea();
	int GetDistances();
	float GetAverageRoomArea();
};