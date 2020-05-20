#pragma once

#include <vector>
#include "LayoutRoom.h"
#include "Constants.h"

class Layout
{
public:
	DLLExport Layout(int NumRooms);
	Layout() {};

	// name of vertex is index of vector!
	std::vector<LayoutRoom> Rooms;
	std::vector<bool> LaidOutVertices;
	DLLExport float GetEnergy();
	DLLExport int GetIntersectionArea();
	DLLExport bool isComplete();
	int GetDistances();
	float GetAverageRoomArea();
};