#pragma once

#include <vector>
#include "LayoutRoom.h"
#include "Constants.h"

class Layout
{
public:
	DLLExport Layout(int NumRooms, int nextChainIndex);
	Layout() {};

	// name of vertex is index of vector!
	std::vector<LayoutRoom> Rooms;
	std::vector<bool> LaidOutVertices;
	int NextChainIndex;
	DLLExport float GetEnergy();
	DLLExport int GetIntersectionArea();
	DLLExport bool isComplete();
	int GetDistances();
	float GetAverageRoomArea();
};