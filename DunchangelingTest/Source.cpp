#include <iostream>
#include "../Dunchangeling/Graph.h"
#include <string>

void main()
{
	Graph graph;
	graph.addEdge(1, 2, false);
	graph.addEdge(1, 3, false);
	graph.addEdge(2, 4, false);
	graph.addEdge(3, 4, false);
	graph.addEdge(4, 5, false);
	graph.addEdge(3, 6, false);
	graph.addEdge(6, 7, false);
	graph.addEdge(5, 7, false);

	graph.shortestPath(1, 6);

	std::string dot = graph.printAsDot();

	std::cout << dot << std::endl;

	getchar();
}