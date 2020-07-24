#include "../Dunchangeling/TestDLL.h"
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"
#include "../Dunchangeling/GraphUtils.cpp"
#include "../Dunchangeling/GeneticAlgorithmUtils.h"
#include "../Dunchangeling/GraphToMap.h"
#include "../Dunchangeling/GeneticAlgorithm.h"
#include "../Dunchangeling/ProductionRules.h"
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
	DungeonProperties props;
	props.NumRooms = 20;
	props.NumSpecialRooms = 1;
	props.FlankingRoutes = false;
	props.OpponentTypes.emplace(1, OpponentInfo(1, 1));
	props.OpponentTypes.emplace(2, OpponentInfo(2, 2));
	props.OpponentTypes.emplace(3, OpponentInfo(3, 3));

	ProductionRules * gaFunctions2 = new ProductionRules();

	GeneticAlgorithm ga(GeneticAlgorithmProperties(), gaFunctions2, props);
	ga.generateInitialPopulation(InitMode::EIM_PATH_THREE);
	ga.run();
	Graph graph = (*ga.CurrentPopBuffer)[0];


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
		1, 3, 1,
		1, 1, 1,
		1, 1, 1
	};

	unsigned int room4[] =
	{
		1, 1, 1, 1, 1,
		1, 3, 3, 3, 1,
		1, 1, 1, 1, 1
	};

	unsigned int room5[] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 0, 1, 1,
		0, 0, 1, 1
	};

	unsigned int room6[] = {
		1, 1, 0, 0,
		1, 1, 0, 0,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 0, 0,
		1, 1, 0, 0
	};

	unsigned int room7[] = {
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1
	};

	unsigned int room8[] = {
		1, 1, 1,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 1, 1,
	};

	Grid grid(4, 4, room);
	Grid grid2(2, 2, room2);
	Grid grid3(3, 5, room3);
	Grid grid4(5, 3, room4);
	Grid grid5(4, 4, room5);
	Grid grid6(4, 6, room6);
	Grid grid7(1, 9, room7);
	Grid grid8(9, 1, room7);
	Grid grid9(3, 8, room8);

	std::vector<Room> rooms = { Room(grid), Room(grid2), Room(grid3), Room(grid4), Room(grid5), Room(grid6) };
	GraphToMap::RoomCollection roomCollection(rooms);

	BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(graph);
	Chains chains = GeneticAlgorithmUtils::ChainDecomposition(bg);

	GraphToMap::MapGenerator mg(roomCollection, chains, bg);
	std::vector<Layout> layoutHistory;
	Layout finalLayout = mg.GenerateLayout(bg, layoutHistory);

	DungeonGrid griddy = GraphToMap::LayoutToSingleGrid(finalLayout);

	*data = griddy.DungeonArray;
	*xSize = griddy.XSize;
	*ySize = griddy.YSize;
	*numData = griddy.numData;

	return griddy.XSize * griddy.YSize * griddy.numData;
}