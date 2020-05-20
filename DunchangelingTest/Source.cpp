#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>
#include <random>
#include "../Dunchangeling/GeneticAlgorithm.h" 
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"
#include "../Dunchangeling/GraphUtils.cpp"
#include "../Dunchangeling/GeneticAlgorithmUtils.h"
#include "../Dunchangeling/GraphToMap.h"

#include "DrawGrid.h"
#include <SFML/Graphics.hpp>

//#include <SFML/Graphics.hpp>

using namespace GeneticAlgorithmUtils;

void generateTestGraphs(Graph& graph1, Graph& graph2, GeneticAlgorithm& ga)
{
	PopId id1 = ga.requestId();
	PopId id2 = ga.requestId();
	PopId id3 = ga.requestId();
	PopId id4 = ga.requestId();
	PopId id5 = ga.requestId();
	PopId id6 = ga.requestId();
	PopId id7 = ga.requestId();

	Graph father;
	father.addEdge(id1, id2, false);
	father.addEdge(id1, id3, false);
	father.addEdge(id2, id4, false);
	father.addEdge(id3, id4, false);
	father.addEdge(id4, id5, false);
	father.addEdge(id3, id6, false);
	father.addEdge(id6, id7, false);
	father.addEdge(id5, id7, false);

	Graph mother;

	PopId id8 = ga.requestId();
	PopId id9 = ga.requestId();
	PopId id10 = ga.requestId();
	PopId id11 = ga.requestId();
	PopId id12 = ga.requestId();
	PopId id13 = ga.requestId();
	PopId id14 = ga.requestId();
	PopId id15 = ga.requestId();

	mother.addEdge(id8, id9, false);
	mother.addEdge(id9, id10, false);
	mother.addEdge(id9, id11, false);
	mother.addEdge(id11, id12, false);
	mother.addEdge(id12, id13, false);
	mother.addEdge(id11, id14, false);
	mother.addEdge(id11, id15, false);

	std::string motherDot = mother.printAsDot();
	std::string fatherDot = father.printAsDot();

	std::cout << "Father " << fatherDot << std::endl;
	std::cout << "Mother " << motherDot << std::endl;

	graph1 = father;
	graph2 = mother;
}

void generateNonPlanarGraph(Graph& graph, GeneticAlgorithm& ga)
{
	PopId id0 = ga.requestId();
	PopId id1 = ga.requestId();
	PopId id2 = ga.requestId();
	PopId id3 = ga.requestId();
	PopId id4 = ga.requestId();

	graph.addEdge(id0, id1, false);
	graph.addEdge(id0, id2, false);
	graph.addEdge(id0, id3, false);
	graph.addEdge(id0, id4, false);
	graph.addEdge(id1, id2, false);
	graph.addEdge(id1, id3, false);
	graph.addEdge(id1, id4, false);
	graph.addEdge(id2, id3, false);
	graph.addEdge(id2, id4, false);
	graph.addEdge(id3, id4, false);

	std::cout << graph << std::endl;
}

void generateDecompTestGraph(Graph& graph, GeneticAlgorithm& ga)
{
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
}

void PlanarityCheck()
{
	GeneticAlgorithm ga;
	Graph graph1, graph2;
	generateNonPlanarGraph(graph1, ga);


	bool test = graph1.IsPlanar();

	if (test)
	{
		std::cout << "This is planar.\n";
	}
	else
	{
		std::cout << "Not planar.\n";
	}
}

void testGraphSplit()
{
	GeneticAlgorithm ga;
	Graph mother, father, part1, part2, part3, part4;
	generateTestGraphs(father, mother, ga);

	graph_splitGraph(father, part1, part2);
	graph_splitGraph(mother, part3, part4);

	std::string dotPart1 = part1.printAsDot();
	std::string dotPart2 = part2.printAsDot();
	std::string dotPart3 = part3.printAsDot();
	std::string dotPart4 = part4.printAsDot();

	Graph fusedGraph = graph_mate(part1, part4, ga);
	std::string fusedGraphDot = fusedGraph.printAsDot();

	std::cout << "Part1: \n" << dotPart1 << std::endl;
	std::cout << "Part2: \n" << dotPart4 << std::endl;
	std::cout << "Fused: \n" << fusedGraphDot << std::endl;
}

void testRandomGraph()
{
	GeneticAlgorithm ga;
	Graph randG = graph_generateRandomGraph(10, ga);
	std::string randdot = randG.printAsDot();
	std::cout << "Rand: \n" << randdot << std::endl;
}

void testRandomGraph2()
{
	GeneticAlgorithm ga;
	Graph randG = graph_generateRandomGraphWilson(10, 13,ga);
	std::string randdot = randG.printAsDot();
	std::cout << "Rand2: \n" << randdot << std::endl;
}

//RoomShape createRoomShape(Grid& grid)
//{
//	int ySize = grid.YSize;
//	int xSize = grid.XSize;
//
//	RoomShape roomShape;
//
//	for (int y = 0; y < ySize; y++)
//	{
//		for (int x = 0; x < xSize; x++)
//		{
//			int val = grid.Get(x, y);
//			if (val == GRID_FILLED_NORMAL)
//			{
//				roomShape.tiles.push_back(Tile(x, y, COLOR_FILLED));
//			}
//			else if(val == GRID_CONFIG_SPACE)
//			{
//				roomShape.tiles.push_back(Tile(x, y, COLOR_CONFIG_SPACE));
//			}
//			else if (val == (GRID_CONFIG_SPACE | GRID_FILLED_NORMAL))
//			{
//				roomShape.tiles.push_back(Tile(x, y, COLOR_CONFIG_FILLED));
//			}
//		}
//	}
//
//	return roomShape;
//}

int main()
{
	Graph g1, g2, g3;
	GeneticAlgorithm ga;

	generateDecompTestGraph(g1, ga);
	generateTestGraphs(g2, g3, ga);

	std::cout << g1 << "\n";

	BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(g2);

	//GraphUtils::GraphToDot(bg);
	GeneticAlgorithmUtils::GetGraphFaces(bg);
	Chains chains = GeneticAlgorithmUtils::ChainDecomposition(bg);

	GeneticAlgorithmUtils::GraphChainsDot(bg, chains);

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

	Grid grid(4, 4, room);
	Grid grid2(2, 2, room2);

	std::vector<Room> rooms = { Room(grid), Room(grid2) };

	GraphToMap::RoomCollection roomCollection(rooms);

	GraphToMap::MapGenerator mg(roomCollection, chains, bg);
	Layout lcool(boost::num_vertices(bg));
	std::vector<std::pair<Layout, std::string>> debugLayout;
	auto layouties = mg.AddChain(lcool, chains[0], bg, 5, 50, 4, 0.6f, 0.2f, debugLayout);

	Layout layout(7);
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
	layout.Rooms[2].Neighbours = { 0, 3};

	layout.Rooms[3].Room = roomCollection[1];
	layout.Rooms[3].VertexID = 3;
	layout.Rooms[3].PosX = 9;
	layout.Rooms[3].PosY = -1;
	layout.Rooms[3].Neighbours = { 1, 2 };

	//std::cout << mg.IsLayoutValid(layout);

	sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");

	LayoutShape lShape(layouties[0], 10, 10, 16);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		lShape.DrawLayoutShape(window);
		window.display();
	}

	//PlanarityCheck();

	//GeneticAlgorithm ga(10, 100);
	//ga.generateInitialPopulation(8, 9, 3);
	//ga.run();

	//ga.currentGenerationToFile("C:/Users/Bastian/Documents/MasterStuff");

	//Graph graph1 = graph_generateRandomGraphWilson(10, randomNumber(9, 14), ga);
	//Graph graph2 = graph_generateRandomGraphWilson(10, randomNumber(9, 14), ga);

	//std::cout << graph1 << std::endl;

	//Graph matedGraph = graph_crossover(graph1, graph2);
	//matedGraph.generateGraphImage();

	//matedGraph.writeToFile();

	//std::cout << "Rand2: \n" << matedGraph << std::endl;

	getchar();
	return 0;
}