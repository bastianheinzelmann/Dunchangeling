#include "../Dunchangeling/TestDLL.h"
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"
#include "../Dunchangeling/GraphUtils.cpp"
#include "../Dunchangeling/GeneticAlgorithmUtils.h"
#include "../Dunchangeling/GraphToMap.h"
#include "../Dunchangeling/GeneticAlgorithm.h"
#include "../Dunchangeling/ProductionRules.h"
#include "../Dunchangeling/EntryEndCrossover.h"
#include "Functions.h"

std::vector<Room> readConcatArray(int concatArray[])
{
	std::vector<Room> rooms;

	int currentIndex = 0;
	int size = concatArray[currentIndex];

	++currentIndex;

	while (currentIndex < size)
	{
		int width;
		int height;

		width = concatArray[currentIndex];
		++currentIndex;
		height = concatArray[currentIndex];

		++currentIndex;
		unsigned int * roomArray = new unsigned int[width * height];

		int roomEnd = currentIndex + width * height;
		for (int i = 0; currentIndex < roomEnd; currentIndex++)
		{
			roomArray[i] = concatArray[currentIndex];
			++i;
		}

		Grid grid(width, height, roomArray);
		rooms.push_back(Room(grid));
	}

	return rooms;
}

int generateLayout(int ** data, int* xSize, int* ySize, int* numData, UDungeonProperties dprops, int concatenatedRoomArray[], UGeneticAlgorithmProperties uGaProps)
{
	DungeonProperties props;
	props.NumRooms = dprops.NumRooms;
	props.NumSpecialRooms = dprops.NumSpecialRooms;
	props.FlankingRoutes = dprops.FlankingRoutes;
	props.branchingFactor = dprops.branchingFactor;
	props.SpecialIsDeadEnd = dprops.SpecialIsDeadEnd;
	props.EndroomDeadEnd = dprops.EndRoomDeadEnd;
	props.OpponentTypes.emplace(1, OpponentInfo(1, 1));
	props.OpponentTypes.emplace(2, OpponentInfo(2, 2));
	props.OpponentTypes.emplace(3, OpponentInfo(3, 3));

	GeneticAlgorithmProperties gaProps;
	gaProps.convergenceBorder = uGaProps.convergenceBorder;
	gaProps.crossoverRate = uGaProps.crossoverRate;
	gaProps.doCrossover = uGaProps.doCrossover;
	gaProps.elitismRate = uGaProps.elitismRate;
	gaProps.populationSize = uGaProps.populationSize;
	gaProps.maxGenerations = uGaProps.populationSize;

	IGAFunctions * gaFunctions = nullptr;

	if (uGaProps.mutationType == EDGE_MUTATIONS)
	{
		gaFunctions = new EntryEndCrossover();
	}
	else if (uGaProps.mutationType == PRODUCTION_MUTATIONS)
	{
		gaFunctions = new ProductionRules();
	}
	else
	{
		assert("no valid mutation type");
	}

	GeneticAlgorithm ga(gaProps, gaFunctions, props);

	InitMode initMode;

	if (uGaProps.initMode == INIT_PATH)
	{
		initMode = InitMode::EIM_PATH;
	}
	else if (uGaProps.initMode == INIT_RANDOM)
	{
		initMode = InitMode::EIM_RANDOM;
	}

	ga.generateInitialPopulation(initMode);


	ga.run();
	Graph graph = (*ga.CurrentPopBuffer)[0];

	auto rooms = readConcatArray(concatenatedRoomArray);
	assert(rooms.size() > 0);

	GraphToMap::RoomCollection roomCollection(rooms);

	BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(graph);
	Chains chains = GeneticAlgorithmUtils::ChainDecomposition(bg);

	GraphToMap::MapGenerator mg(roomCollection, chains, bg);
	std::vector<Layout> layoutHistory;
	Layout finalLayout = mg.GenerateLayout(bg, layoutHistory);

	DungeonGrid griddy = GraphToMap::LayoutToSingleGrid(finalLayout);

	// delete roomgrids
	for (auto i : roomCollection.Rooms)
	{
		delete[] i.RoomGrid.gridArray;
	}

	delete gaFunctions;

	*data = griddy.DungeonArray;
	*xSize = griddy.XSize;
	*ySize = griddy.YSize;
	*numData = griddy.numData;

	return griddy.XSize * griddy.YSize * griddy.numData;
}