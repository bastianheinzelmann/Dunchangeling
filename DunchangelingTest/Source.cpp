#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>
#include <random>

int main()
{
	//std::random_device dev;
	//std::mt19937* rng = new std::mt19937(dev());
	//std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

	Graph graph;
	graph.addEdge(1, 2, false);
	graph.addEdge(1, 3, false);
	graph.addEdge(2, 4, false);
	graph.addEdge(3, 4, false);
	graph.addEdge(4, 5, false);
	graph.addEdge(3, 6, false);
	graph.addEdge(6, 7, false);
	graph.addEdge(5, 7, false);

	Graph part1, part2;
	graph.splitGraph(3, 4, part1, part2);

	graph.shortestPath(1, 6);

	std::string dot = graph.printAsDot();

	std::string dotPart1 = part1.printAsDot();
	std::string dotPart2 = part2.printAsDot();

	std::cout << dot << std::endl;

	std::cout << "Part1: \n" << dotPart1 << std::endl;
	std::cout << "Part2: \n" << dotPart2 << std::endl;

	getchar();

	return 0;
}