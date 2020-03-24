#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>
#include <random>
#include "../Dunchangeling/GeneticAlgorithm.h" 

using namespace GraphUtils;

int main()
{
	//std::random_device dev;
	//std::mt19937* rng = new std::mt19937(dev());
	//std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

	Graph father;
	father.addEdge(1, 2, false);
	father.addEdge(1, 3, false);
	father.addEdge(2, 4, false);
	father.addEdge(3, 4, false);
	father.addEdge(4, 5, false);
	father.addEdge(3, 6, false);
	father.addEdge(6, 7, false);
	father.addEdge(5, 7, false);

	Graph mother;
	mother.addEdge(8, 9, false);
	mother.addEdge(9, 10, false);
	mother.addEdge(9, 11, false);
	mother.addEdge(11, 12, false);
	mother.addEdge(12, 13, false);
	mother.addEdge(11, 14, false);
	mother.addEdge(11, 15, false);

	std::string motherDot = mother.printAsDot();
	std::string fatherDot = father.printAsDot();

	Graph part1, part2, part3, part4;

	std::cout << "Mother " << motherDot << std::endl;
	std::cout << "Father " << fatherDot << std::endl;

	graph_splitGraph(father, part1, part2);
	graph_splitGraph(mother, part3, part4);

	Graph fusedGraph = graph_mate(part1, part4);
	std::string fusedPart = fusedGraph.printAsDot();

	std::string dotPart1 = part1.printAsDot();
	std::string dotPart2 = part2.printAsDot();
	std::string dotPart3 = part3.printAsDot();
	std::string dotPart4 = part4.printAsDot();

	std::cout << "Part1: \n" << dotPart1 << std::endl;
	std::cout << "Part2: \n" << dotPart4 << std::endl;
	std::cout << "Fused: \n" << fusedPart << std::endl;

	GeneticAlgorithm ga;

	Graph randGraph = graph_generateRandomGraph(16, ga);
	std::string randdot = randGraph.printAsDot();

	std::cout << "Rand: \n" << randdot << std::endl;

	getchar();

	return 0;
}