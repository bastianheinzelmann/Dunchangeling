#pragma once

#include <vector>
#include "LayoutRoom.h"

class Layout
{
public:
	std::vector<LayoutRoom> Rooms;
	std::vector<int> LaidOutVertices;
};