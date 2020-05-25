#include "../Dunchangeling/TestDLL.h"
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"
#include "../Dunchangeling/GraphUtils.cpp"
#include "../Dunchangeling/GeneticAlgorithmUtils.h"
#include "../Dunchangeling/GraphToMap.h"
#include "Functions.h"

int addWrapper(int num1, int num2)
{
	return add(num1, num2);
}

int multiplyWrapper(int num1, int num2)
{
	return multiply(num1, num2);
}

int substractWrapper(int num1, int num2)
{
	return substract(num1, num2);
}

int divideWrapper(int num1, int num2)
{
	return divide(num1, num2);
}

int generateLayout(int ** data, int * size)
{
	unsigned int room[] =
	{
		0, 0, 1, 1,
		0, 0, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1
	};

	unsigned int room2[] =
	{
		1, 1,
		1, 1
	};

	unsigned int room3[] =
	{
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	unsigned int room4[] =
	{
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1
	};

	unsigned int room5[] = {
		0, 0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 0, 0
	};

	//unsigned int room5[] = {
	//	1, 1, 1, 1,
	//	1, 0, 0, 1,
	//	1, 0, 0, 1,
	//	1, 1, 1, 1
	//};

	Grid grid(4, 4, room);
	Grid grid2(2, 2, room2);
	Grid grid3(3, 5, room3);
	Grid grid4(5, 3, room4);
	Grid grid5(7, 8, room5);

	std::vector<Room> rooms = { Room(grid), Room(grid2), Room(grid3), Room(grid4), Room(grid5) };

	GraphToMap::RoomCollection roomCollection(rooms);

	Layout layout(7, 0);
	layout.LaidOutVertices = { true, true, true, true, false , false, false };

	layout.Rooms[0].Room = roomCollection[1];
	layout.Rooms[0].VertexID = 0;
	layout.Rooms[0].PosX = 6;
	layout.Rooms[0].PosY = 0;
	layout.Rooms[0].Neighbours = { 1, 2 };

	layout.Rooms[1].Room = roomCollection[1];
	layout.Rooms[1].VertexID = 1;
	layout.Rooms[1].PosX = 8;
	layout.Rooms[1].PosY = 1;
	layout.Rooms[1].Neighbours = { 0, 3 };

	layout.Rooms[2].Room = roomCollection[0];
	layout.Rooms[2].VertexID = 2;
	layout.Rooms[2].PosX = 5;
	layout.Rooms[2].PosY = -4;
	layout.Rooms[2].Neighbours = { 0, 3 };

	layout.Rooms[3].Room = roomCollection[1];
	layout.Rooms[3].VertexID = 3;
	layout.Rooms[3].PosX = 9;
	layout.Rooms[3].PosY = -1;
	layout.Rooms[3].Neighbours = { 1, 2 };

	DungeonGrid griddy = GraphToMap::LayoutToSingleGrid(layout);

	*data = griddy.DungeonArray;
	*size = griddy.XSize * griddy.YSize * 5;

	return griddy.XSize * griddy.YSize * 5;
}