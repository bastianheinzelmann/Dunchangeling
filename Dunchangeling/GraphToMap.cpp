#include "GraphToMap.h"

RoomCollection::RoomCollection(unsigned int roomNum, Room rooms[])
{
	this->rooms = rooms;
	this->roomNum = roomNum;
}

void calculateConfigSpaces(RoomCollection& roomCollection)
{
	for (int i = 0; i < roomCollection.roomNum; i++)
	{
		Room& currentRoom = roomCollection[i];
		for (int j = 0; i < roomCollection.roomNum; j++)
		{
			
		}
	}
}