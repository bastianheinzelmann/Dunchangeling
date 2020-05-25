#include "../Dunchangeling/TestDLL.h"
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"
#include "../Dunchangeling/GraphUtils.cpp"
#include "../Dunchangeling/GeneticAlgorithmUtils.h"
#include "../Dunchangeling/GraphToMap.h"
#include "../Dunchangeling/GeneticAlgorithm.h"
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

int generateLayout(int ** data, int* xSize, int* ySize, int* numData)
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

#pragma region MyRegion

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
#pragma endregion

	GeneticAlgorithm ga;
	Graph graph;

	PopId id1 = ga.requestId();
	PopId id2 = ga.requestId();
	PopId id3 = ga.requestId();
	PopId id4 = ga.requestId();
	PopId id5 = ga.requestId();
	PopId id6 = ga.requestId();
	PopId id7 = ga.requestId();
	PopId id8 = ga.requestId();
	PopId id9 = ga.requestId();
	PopId id10 = ga.requestId();
	PopId id11 = ga.requestId();
	PopId id12 = ga.requestId();
	PopId id13 = ga.requestId();

	graph.addEdge(id1, id2, false);
	graph.addEdge(id2, id3, false);
	graph.addEdge(id2, id10, false);
	graph.addEdge(id3, id4, false);
	graph.addEdge(id3, id7, false);
	graph.addEdge(id4, id5, false);
	graph.addEdge(id4, id5, false);
	graph.addEdge(id5, id6, false);
	graph.addEdge(id6, id7, false);
	graph.addEdge(id7, id8, false);
	graph.addEdge(id8, id9, false);
	graph.addEdge(id9, id10, false);
	graph.addEdge(id9, id11, false);
	graph.addEdge(id11, id12, false);
	graph.addEdge(id11, id13, false);

	BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(graph);
	Chains chains = GeneticAlgorithmUtils::ChainDecomposition(bg);

	GraphToMap::MapGenerator mg(roomCollection, chains, bg);
	Layout finalLayout = mg.GenerateLayout(bg);

	DungeonGrid griddy = GraphToMap::LayoutToSingleGrid(finalLayout);

	*data = griddy.DungeonArray;
	*xSize = griddy.XSize;
	*ySize = griddy.YSize;
	*numData = griddy.numData;

	return griddy.XSize * griddy.YSize * 5;
}