#pragma once

#include "Room.h"
#include "Constants.h"
#include <vector>

struct RoomCollection{
	DLLExport RoomCollection(std::vector<Room> rooms);
	DLLExport RoomCollection() {};

	std::vector<Room> Rooms;

	Room& operator[](std::size_t idx) { return Rooms[idx]; }
	const Room& operator[](std::size_t idx) const { return Rooms[idx]; }
};

void calculateConfigSpaces(RoomCollection& roomCollection);