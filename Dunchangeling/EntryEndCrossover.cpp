#include "EntryEndCrossover.h"
#include "GeneticAlgorithmUtils.h"

Graph EntryEndCrossover::Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga)
{
	Graph parent1part1, parent1part2, parent2part1, parent2part2;

	graph_splitGraph(parent1, parent1part1, parent1part2);
	graph_splitGraph(parent2, parent2part1, parent2part2);

	Graph entryGraph;
	Graph endGraph;

	if (parent1part1.attributes.entryIndex > -1)
	{
		entryGraph = parent1part1;
	}
	else if (parent1part2.attributes.entryIndex > -1)
	{
		entryGraph = parent1part2;
	}
	else
	{
		assert(true);
	}

	if (parent2part1.attributes.endIndex > -1)
	{
		endGraph = parent2part1;
	}
	else if (parent2part2.attributes.endIndex > -1)
	{
		endGraph = parent2part2;
	}
	else
	{
		assert(true);
	}

	Graph graph = graph_mate(entryGraph, endGraph, ga);

	for (int i = 0; i < graph.vertices.size(); i++)
	{
		graph.vertices[i].vertexID = ga.requestId();
	}

	assert(integrityCheck(graph));

	return graph;
}

void EntryEndCrossover::Mutate(Graph & graph, GeneticAlgorithm & ga)
{
	//for (int i = 0; i < graph.vertices.size(); i++)
	//{
	//	int p = randomNumber(0, 100);
	//	//std::cout << "P: " << p << std::endl;
	//	if (p < 7)
	//	{
	//		graph_removeEdgeMutation(graph, graph.vertices[i]);
	//	}
	//	else if (p < 14)
	//	{
	//		graph_addEdgeMutation(graph, graph.vertices[i]);
	//	}
	//	else if (p < 21)
	//	{
	//		//graph_addVertexMutation(graph, graph.vertices[i], ga);
	//	}
	//	else if (p < 28)
	//	{
	//		graph_swapEndMutation(graph, ga);
	//	}
	//	else if (p < 35)
	//	{
	//		graph_swapEntryMutation(graph, ga);
	//	}

	//	assert(integrityCheck(graph));
	//}
	int p = randomNumber(0, 100);
	int vertexIndex = randomNumber(0, graph.vertices.size() - 1);
	//std::cout << "P: " << p << std::endl;
	if (p < 7)
	{
		graph_removeEdgeMutation(graph, graph.vertices[vertexIndex]);
	}
	else if (p < 14)
	{
		graph_addEdgeMutation(graph, graph.vertices[vertexIndex]);
	}
	else if (p < 21)
	{
		//graph_addVertexMutation(graph, graph.vertices[i], ga);
	}
	else if (p < 28)
	{
		graph_swapEndMutation(graph, ga);
	}
	else if (p < 35)
	{
		graph_swapEntryMutation(graph, ga);
	}

	assert(integrityCheck(graph));
}