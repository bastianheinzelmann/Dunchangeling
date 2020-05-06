#include "GraphToMap.h"

RoomCollection::RoomCollection(std::vector<Room> rooms)
{
	this->Rooms = rooms;
}

// should make sure all ConfigGrids in rooms are empty before so that ids are correct
void calculateConfigSpaces(RoomCollection& roomCollection)
{
	for (int i = 0; i < roomCollection.Rooms.size(); i++)
	{
		Room& currentRoom = roomCollection[i];
		currentRoom.RoomID = i;
		for (int j = 0; i < roomCollection.Rooms.size(); j++)
		{
			Room& otherRoom = roomCollection[i];
			currentRoom.CalculateConfigGrid(otherRoom);
		}
	}
}