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
#include "../Dunchangeling/EntryEndCrossover.h"

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

void generateFuckGraph(Graph& graph, GeneticAlgorithm& ga)
{
	PopId id0 = ga.requestId();
	PopId id1 = ga.requestId();
	PopId id2 = ga.requestId();
	PopId id3 = ga.requestId();
	PopId id4 = ga.requestId();
	PopId id5 = ga.requestId();
	PopId id6 = ga.requestId();
	PopId id7 = ga.requestId();

	graph.addEdge(id7, id2, false);
	graph.addEdge(id2, id5, false);
	graph.addEdge(id5, id3, false);
	graph.addEdge(id5, id4, false);
	graph.addEdge(id3, id4, false);
	graph.addEdge(id4, id0, false);
	graph.addEdge(id0, id6, false);
	graph.addEdge(id6, id1, false);
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

void generateTestLayout(Layout & layout, GraphToMap::RoomCollection & roomCollection)
{
	layout = Layout(7, 0);
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
}

int main()
{
	//Graph g1, g2, g3;
	//GeneticAlgorithm ga;


	//std::cout << g1 << "\n";

	//BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(g1);

	//GraphUtils::GraphToDot(bg);
	//GeneticAlgorithmUtils::GetGraphFaces(bg);
	//Chains chains = GeneticAlgorithmUtils::ChainDecomposition(bg);
	//GeneticAlgorithmUtils::GraphChainsDot(bg, chains);

	EntryEndCrossover * gaFunctions = new EntryEndCrossover();

	DungeonProperties props;
	props.NumRooms = 8;
	props.NumSpecialRooms = 1;

	GeneticAlgorithm ga(100, 1000, gaFunctions, props);

	Graph graph1 = graph_generateRandomGraphWilson(10, randomNumber(9, 14), ga);
	Graph graph2 = graph_generateRandomGraphWilson(10, randomNumber(9, 14), ga);

	//Graph graphDecomp;
	//generateDecompTestGraph(graphDecomp, ga);

	//std::cout << graph1 << std::endl;
	//std::cout << graph2 << std::endl;

	//Graph matedGraph = graph_crossover(graph1, graph2, ga);
	//std::cout << "Mated Graph\n" << matedGraph << std::endl;

	ga.generateInitialPopulation(8, 9, 3);
	ga.generateInitialPopulation(12, 15, 3);
	ga.run();
	Graph gaGraph = (*ga.CurrentPopBuffer)[0];
	std::cout << gaGraph;

	//Graph gaGraph;
	//generateFuckGraph(gaGraph, ga);

	BoostGraph bg = GeneticAlgorithmUtils::ConvertToBoostGraph(gaGraph);
	GraphToDot(bg);
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
	Grid grid5(4, 4, room5);
	Grid grid6(4, 6, room6);

	std::vector<Room> rooms = { Room(grid), Room(grid2), Room(grid3), Room(grid4), Room(grid5), Room(grid6)};

	GraphToMap::RoomCollection roomCollection(rooms);

	GraphToMap::MapGenerator mg(roomCollection, chains, bg);
	std::vector<std::pair<Layout, std::string>> debugLayout;

	Layout finalLayout = mg.GenerateLayout(bg);

	//Layout testLayout;
	//generateTestLayout(testLayout, roomCollection);

	DungeonGrid griddy = GraphToMap::LayoutToSingleGrid(finalLayout);
	FinalGrid* finalGrid = new FinalGrid(griddy);

	sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Blue);
		//lShape.DrawLayoutShape(window);
		finalGrid->Draw(window);
		window.display();
	}

	//PlanarityCheck();

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