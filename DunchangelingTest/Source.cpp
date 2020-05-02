#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>
#include <random>
#include "../Dunchangeling/GeneticAlgorithm.h" 
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/Constants.h"

#include <SFML/Graphics.hpp>

//#include <SFML/Graphics.hpp>

using namespace GraphUtils;

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

int main()
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

	Grid grid(4, 4, room);
	Grid grid2(2, 2, room2);

	Room actualRoom(grid);
	Room squareRoom(grid2);

	std::cout << grid;
	std::cout << actualRoom.RoomHull << "\n";
	std::cout << squareRoom.RoomHull << "hihi\n";

	std::cout << "Hello \n" << actualRoom.CalculateConfigGrid(squareRoom);
	std::cout << "Hello \n" << squareRoom.CalculateConfigGrid(actualRoom);

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
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