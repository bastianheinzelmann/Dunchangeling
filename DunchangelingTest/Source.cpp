#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>
#include <random>
#include "../Dunchangeling/GeneticAlgorithm.h" 

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

	Graph fusedGraph = graph_mate(part1, part4);
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
	GeneticAlgorithm ga(10);
	ga.generateInitialPopulation(8, 9, 3);
	ga.currentGenerationToFile("C:/Users/Bastian/Documents/MasterStuff");

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