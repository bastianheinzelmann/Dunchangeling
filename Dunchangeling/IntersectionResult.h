#pragma once

#include <vector>

struct IntersectionResult
{
	int XPos;
	int YPos;
	std::vector<int> RoomsNotConnected;
};