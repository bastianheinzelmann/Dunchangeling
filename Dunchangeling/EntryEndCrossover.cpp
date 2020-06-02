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
	int p = randomNumber(0, 100);
	int vertexIndex = randomNumber(0, graph.vertices.size() - 1);
	//std::cout << "P: " << p << std::endl;
	//if (p < 7)
	//{
	//	graph_removeEdgeMutation(graph, graph.vertices[vertexIndex]);
	//}
	//else if (p < 14)
	//{
	//	graph_addEdgeMutation(graph, graph.vertices[vertexIndex]);
	//}
	//else if (p < 21)
	//{
	//	graph_swapEndMutation(graph, ga);
	//}
	//else if (p < 28)
	//{
	//	graph_swapEntryMutation(graph, ga);
	//}
	//else if (p < 35)
	//{
	//	graph_addTreasure(graph, ga);
	//}
	//else if (p < 42)
	//{
	//	graph_removeTreasure(graph, ga);
	//}

	if (p < 17)
	{
		graph_removeEdgeMutation(graph, graph.vertices[vertexIndex]);
	}
	else if (p < 34)
	{
		graph_addEdgeMutation(graph, graph.vertices[vertexIndex]);
	}
	else if (p < 21)
	{
		graph_swapEndMutation(graph, ga);
	}
	else if (p < 51)
	{
		graph_swapEntryMutation(graph, ga);
	}
	else if (p < 68)
	{
		graph_addTreasure(graph, ga);
	}
	else if (p < 85)
	{
		graph_removeTreasure(graph, ga);
	}

	assert(integrityCheck(graph));
}

void EntryEndCrossover::CalculateFitness(Graph & graph, GeneticAlgorithm & ga)
{
	float fitness;

	std::vector<unsigned int> path = graph.shortestPath(graph.vertices[graph.attributes.entryIndex].vertexID, graph.vertices[graph.attributes.endIndex].vertexID);

	int specialRoomsInCritPath = 0;
	int specialRoomsNotInCritPath = 0;

	std::vector<bool> visited(graph.vertices.size(), false);

	for (int i = 0; i < path.size(); i++)
	{
		visited[path[i]] = true;
		if (graph.vertices[path[i]].attributes.treasureRoom)
		{
			++specialRoomsInCritPath;
		}
	}

	for (int i = 0; i < graph.vertices.size(); i++)
	{
		if (!visited[i])
		{
			if (graph.vertices[i].attributes.treasureRoom)
			{
				++specialRoomsNotInCritPath;
			}
		}
	}

	int numDeadEnds = 0;
	int numBrokenDeadends = 0;

	// dead ends: special rooms are dead ends and the end room is a dead end
	for (int i = 0; i < graph.vertices.size(); i++)
	{
		if (graph.vertices[i].attributes.isEndRoom || graph.vertices[i].attributes.treasureRoom)
		{
			++numDeadEnds;
			if (graph.vertices[i].neighbours.size() > 1)
			{
				++numBrokenDeadends;
			}
		}
	}


	int numSpecialRooms = specialRoomsInCritPath + specialRoomsNotInCritPath;

	// between 0 - 10 currently
	int outsideCritPath = graph.vertices.size() - path.size() - specialRoomsNotInCritPath;
	int specialRoomDifference = std::abs(ga.DProperties.NumSpecialRooms - (specialRoomsInCritPath + specialRoomsNotInCritPath));

	float critPathFitness = std::exp((float)outsideCritPath) - 1.0f;
	float specialRoomDiffFitness = std::exp(specialRoomDifference * 5.0f) - 1.0f;

	float deadEndFitness = std::exp((float)numBrokenDeadends) - 1.0f;

	fitness = critPathFitness + specialRoomDiffFitness + deadEndFitness;

	graph.fitness = fitness;
}