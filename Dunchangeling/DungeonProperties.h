#pragma once
#include "OpponentInfo.h"
#include <unordered_map>

struct DungeonProperties
{
	int NumSpecialRooms;
	int NumRooms;
	int NumOpponents;

	bool EndroomDeadEnd = true;
	bool SpecialIsDeadEnd = true;
	bool FlankingRoutes = false;
	int NumFlankingRoutes = 2;
	float branchingFactor = 0;

	std::unordered_map<int, OpponentInfo> OpponentTypes;
};