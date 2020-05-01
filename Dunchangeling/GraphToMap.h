#pragma once

#include "Room.h"
#include "Constants.h"

struct RoomCollection{
	DLLExport RoomCollection(unsigned int roomNum, Room rooms[]);

	int roomNum;
	Room* rooms;

	Room& operator[](std::size_t idx) { return rooms[idx]; }
	const Room& operator[](std::size_t idx) const { return rooms[idx]; }
};

void calculateConfigSpaces(RoomCollection& roomCollection);