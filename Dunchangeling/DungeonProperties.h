#pragma once
#include "OpponentInfo.h"
#include <unordered_map>

struct DungeonProperties
{
	int NumSpecialRooms;
	int NumRooms;
	int NumOpponents;
	std::unordered_map<int, OpponentInfo> OpponentTypes;

	bool EndroomDeadEnd = true;
	bool SpecialIsDeadEnd = true;
	bool FlankingRoutes = false;
	int NumFlankingRoutes = 2;
};