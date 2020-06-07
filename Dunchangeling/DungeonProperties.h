#pragma once

struct DungeonProperties
{
	int NumSpecialRooms;
	int NumRooms;

	bool EndroomDeadEnd = true;
	bool SpecialIsDeadEnd = true;
	bool FlankingRoutes = false;
	int NumFlankingRoutes = 2;
};