#include "GeneticAlgorithm.h"
#include <random>
#include <iostream>
#include <assert.h>
#include <chrono>
#include <math.h>
#include "GeneticAlgorithmUtils.h"

DLLExport int randomNumber(int min, int max)
{
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::default_random_engine re{seed};
	//re.seed(std::chrono::system_clock::now().time_since_epoch().count());
	using Dist = std::uniform_int_distribution<int>;
	static Dist uid{};
	return uid(re, Dist::param_type{ min, max });
}

bool integrityCheck(Graph & graph)
{
	if (graph.attributes.endIndex < 0)
	{
		std::cout << "no end index\n";
		return false;
	}
	if (graph.attributes.entryIndex < 0)
	{
		std::cout << "No entry index\n";
		return false;
	}

	if (!graph.vertices[graph.attributes.endIndex].attributes.isEndRoom)
	{
		std::cout << "End index points to wrong vertex\n";
		return false;
	}

	if (!graph.vertices[graph.attributes.entryIndex].attributes.isEntry)
	{
		std::cout << "Entry index points to wrong vertex\n";
		return false;
	}

	bool hasEntry = false;
	bool hasEnd = false;

	for (int i = 0; i < graph.vertices.size(); i++)
	{
		if (graph.vertices[i].neighbours.size() == 0)
		{
			std::cout << "Vertex with no neighbours\n";
			return false;
		}
		if (graph.vertices[i].attributes.isEntry)
		{
			if (!hasEntry)
			{
				hasEntry = true;
			}
			else
			{
				std::cout << "doubled entry\n";
				return false;
			}
		}
		if (graph.vertices[i].attributes.isEndRoom)
		{
			if (!hasEnd)
			{
				hasEnd = true;
			}
			else
			{
				std::cout << "doubled end\n";
				return false;
			}
		}
	}

	//if (graph.shortestPath(graph.vertices[graph.attributes.entryIndex].vertexID, graph.vertices[graph.attributes.endIndex].vertexID).empty())
	//{
	//	std::cout << "Entry and end are not connected. \n";
	//	return false;
	//}

	return true;
}

DLLExport float randomFloatNumber(float min, float max)
{
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::default_random_engine re{};
	//re.seed(std::chrono::system_clock::now().time_since_epoch().count());
	using Dist = std::uniform_real_distribution<>;
	static Dist ufd{};
	return ufd(re, Dist::param_type{ min, max });
}

DLLExport Graph graph_fuseGraphs(const Graph& graph1, const Graph& graph2, GeneticAlgorithm & ga)
{
	std::vector<Vertex> vertices(graph1.vertices);
	std::vector<Vertex> vertices2(graph2.vertices);

	int offset = vertices.size();

	for (int i = 0; i < vertices2.size(); i++)
	{
		std::unordered_set<int> neighboursWithOffset;
		for (auto j : vertices2[i].neighbours)
		{
			neighboursWithOffset.insert(offset + j);
		}
		vertices2[i].neighbours = neighboursWithOffset;
	}

	vertices.insert(vertices.end(), vertices2.begin(), vertices2.end());

	Graph fusedGraph = Graph(vertices);

	for (int i = 0; i < fusedGraph.vertices.size(); i++)
	{
		if (fusedGraph.vertices[i].attributes.isEntry)
		{
			fusedGraph.attributes.entryIndex = i;
		}
		if (fusedGraph.vertices[i].attributes.isEndRoom)
		{
			fusedGraph.attributes.endIndex = i;
		}
	}

	return fusedGraph;
}

DLLExport Graph graph_mate(Graph& graph1, Graph& graph2, GeneticAlgorithm & ga)
{
	assert(!graph1.empty() && !graph2.empty(), "One graph is empty during crossover");

	// get the names of the splitted graph's broken vertices
	std::vector<int> brokenEdgesGraph1 = graph1.getAllBrokenEdges();
	std::vector<int> brokenEdgesGraph2 = graph2.getAllBrokenEdges();

	// fuse em
	Graph fusedGraph;
	fusedGraph = graph_fuseGraphs(graph1, graph2, ga);

	// shuffle em brokenedges
	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(brokenEdgesGraph2), std::end(brokenEdgesGraph2), rng);

	// last broken edge from graph1 gets last broken edge from graph2 and so on

	while (!brokenEdgesGraph1.empty() || !brokenEdgesGraph2.empty())
	{
		int lastVertex1 = -1;
		int lastVertex2 = -1;

		if (!brokenEdgesGraph1.empty())
		{
			lastVertex1 = brokenEdgesGraph1.back();
			brokenEdgesGraph1.pop_back();
		}


		if (!brokenEdgesGraph2.empty())
		{
			lastVertex2 = brokenEdgesGraph2.back();
			brokenEdgesGraph2.pop_back();
		}


		if(lastVertex2 == -1)
		{
			// select random vertex in graph2
			// this is a serious problem not really because we have graph2 i am kinda silly today and great!
			int randIndex = randomNumber(0, graph2.vertices.size() - 1);
			lastVertex2 = graph2.vertices[randIndex].vertexID;
		}

		if (lastVertex1 == -1)
		{
			int randIndex = randomNumber(0, graph1.vertices.size() - 1);
			lastVertex1 = graph1.vertices[randIndex].vertexID;
		}

		fusedGraph.addEdge(lastVertex1, lastVertex2, false);
	}

	fusedGraph.clearBrokenEdges();

	return fusedGraph;
}

DLLExport void graph_splitGraph(Graph graph, Graph& graph1, Graph& graph2)
{
	int randIndex = randomNumber(0, graph.vertices.size() - 1);

	//TODO get a random neighbour (not yet)
	int randNeighbour = *graph.vertices[randIndex].neighbours.begin();

	//std::cout << "Notice me senpai!!! RandIndex: " << graph.vertices[randIndex].vertexName << " RandNeighbour " << graph.vertices[randNeighbour].vertexName << std::endl;

	//graph.splitGraph(graph.vertices[randIndex].vertexID, graph.vertices[randNeighbour].vertexID, graph1, graph2);
	graph.splitGraph(graph.vertices[graph.attributes.entryIndex].vertexID, graph.vertices[graph.attributes.endIndex].vertexID, graph1, graph2);

	assert(!graph1.empty() && !graph2.empty(), "One graph is empty after split");
}

void GetDeadends(Graph & graph, std::vector<int> & indices)
{
	for (int i = 0; i < graph.vertices.size(); i++)
	{
		if (graph.vertices[i].neighbours.size() == 1)
		{
			indices.push_back(i);
		}
	}
}

DLLExport void graph_addRandomEntry(Graph& graph)
{
	int entryIndex;

	do
	{
		entryIndex = randomNumber(0, graph.vertices.size() - 1);
	} while (graph.attributes.endIndex == entryIndex);

	graph.attributes.entryIndex = entryIndex;
}

DLLExport void graph_addRandomEndRoom(Graph& graph)
{
	int endIndex;
	std::vector<int> deadends;
	GetDeadends(graph, deadends);

	if (deadends.size() > 0)
	{
		endIndex = deadends[randomNumber(0, deadends.size() - 1)];
	}
	else
	{
		do
		{
			endIndex = randomNumber(0, graph.vertices.size() - 1);
		} while (graph.attributes.endIndex == endIndex);
	}
	graph.attributes.endIndex = endIndex;
}

DLLExport Graph graph_crossover(Graph& parent1, Graph& parent2, GeneticAlgorithm& ga)
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
	else if(parent2part2.attributes.endIndex > -1)
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

	//std::cout << "I split\n<< " << parent1 << std::endl;
	//graph_splitGraph(parent1, parent1part1, parent1part2);
	//std::cout << "Part1\n" << parent1part1 << std::endl;
	//std::cout << "Part2\n" << parent1part2 << std::endl;
	//std::cout << "I split\n<< " << parent2 << std::endl;
	//graph_splitGraph(parent2, parent2part1, parent2part2);
	//std::cout << "Part1\n" << parent2part1 << std::endl;
	//std::cout << "Part2\n" << parent2part2 << std::endl;

	//unsigned int partKind1 = 0, partKind2 = 0;
	//if (parent1part1.attributes.entryIndex > -1) partKind1 = partKind1 | 1;
	//if (parent1part1.attributes.endIndex > -1) partKind1 = partKind1 | 2;
	//if (parent1part2.attributes.entryIndex > -1) partKind1 = partKind1 | 4;
	//if (parent1part2.attributes.endIndex > -1) partKind1 = partKind1 | 8;

	//if (parent2part1.attributes.entryIndex > -1) partKind2 = partKind2 | 1;
	//if (parent2part1.attributes.endIndex > -1) partKind2 = partKind2 | 2;
	//if (parent2part2.attributes.entryIndex > -1) partKind2 = partKind2 | 4;
	//if (parent2part2.attributes.endIndex > -1) partKind2 = partKind2 | 8;

	//Graph graph;

	//int partKind = partKind1 | partKind2;

	//assert(partKind != 0);

	//switch (partKind)
	//{
	//case 0b1111: {
	//	graph = graph_mate(parent1part1, parent2part1, ga);
	//	break;
	//}
	//case 0b1001: {
	//	graph = graph_mate(parent1part2, parent2part1, ga);
	//	break;
	//}
	//case 0b0110: {
	//	graph = graph_mate(parent1part2, parent2part1, ga);
	//	break;
	//}
	//case 0b1110: {
	//	if (partKind1 == 0b1100)
	//	{
	//		// need to add random end room to parent1part1
	//		graph_addRandomEndRoom(parent1part1);
	//		graph = graph_mate(parent1part1, parent2part2, ga);
	//	}
	//	else
	//	{
	//		// add random entry to parent2part1
	//		graph_addRandomEntry(parent2part1);
	//		graph = graph_mate(parent1part1, parent2part1, ga);
	//	}
	//	break;
	//}
	//case 0b1101: {
	//	if (partKind1 == 0b1100)
	//	{
	//		// need to add end to parent1part1
	//		graph_addRandomEndRoom(parent1part1);
	//		graph = graph_mate(parent1part1, parent2part1, ga);
	//	}
	//	else
	//	{
	//		// add end roim to parent part 1
	//		graph_addRandomEndRoom(parent2part1);
	//		graph = graph_mate(parent1part1, parent2part1, ga);
	//	}
	//	break;
	//}
	//case 0b0111: {
	//	if (partKind1 == 0b0011)
	//	{
	//		// parent1part2 needs endroom bruh
	//		graph_addRandomEndRoom(parent1part2);
	//		graph = graph_mate(parent1part2, parent2part1, ga);
	//	}
	//	else
	//	{
	//		// add endroo, to parent2part2
	//		graph_addRandomEndRoom(parent2part2);
	//		graph = graph_mate(parent1part2, parent2part2, ga);
	//	}
	//	break;
	//}
	//case 0b1011: {
	//	if (partKind1 == 0b0011)
	//	{
	//		// add entry to parent1part1
	//		graph_addRandomEntry(parent1part1);
	//		graph = graph_mate(parent1part1, parent2part2, ga);
	//	}
	//	else
	//	{
	//		// parent2part2 needs end room
	//		graph_addRandomEndRoom(parent2part2);
	//		graph = graph_mate(parent1part2, parent2part2, ga);
	//	}
	//	break;
	//}
	//default: assert(true, "Did not find crossover case");
	//}

	assert(integrityCheck(graph));

	return graph;
}

DLLExport void graph_addRandomEdges(Graph& graph, int edgeNum)
{
	for (int i = 0; i < edgeNum; i++)
	{
		int randVertexIndex1 = randomNumber(0, graph.vertices.size() - 1);

		int randVertexIndex2;

		do
		{
			randVertexIndex2 = randomNumber(0, graph.vertices.size() - 1);
		} while (randVertexIndex1 == randVertexIndex2);

		graph.addEdge(graph.vertices[randVertexIndex1].vertexID, graph.vertices[randVertexIndex2].vertexID, false);
	}
}

DLLExport Graph graph_generateRandomGraphWilson(int verticesNum, int edgesNum, GeneticAlgorithm& ga)
{
	assert(edgesNum >= verticesNum - 1);

	std::unordered_set<PopId> leftVertices;
	std::vector<PopId> allVertices;
	std::unordered_set<int> visitedVertices;  // already visited vertices by name?

	for (int i = 0; i < verticesNum; i++)
	{
		PopId newVertexId = ga.requestId();
		allVertices.push_back(newVertexId);
		leftVertices.insert(newVertexId);
	}

	Graph graph;

	PopId current_vertex = allVertices[randomNumber(0, verticesNum - 1)];
	visitedVertices.insert(current_vertex);
	leftVertices.erase(current_vertex);

	while (!leftVertices.empty())
	{
		PopId neighbour = allVertices[randomNumber(0, verticesNum - 1)];

		if (visitedVertices.find(neighbour) == visitedVertices.end())
		{
			graph.addEdge(current_vertex, neighbour, false);
			leftVertices.erase(neighbour);
			visitedVertices.insert(neighbour);
		}

		current_vertex = neighbour;
	}

	graph_addRandomEdges(graph, edgesNum - verticesNum);

	int entryVertexIndex = randomNumber(0, verticesNum - 1);
	int endVertexIndex;
	do
	{
		endVertexIndex = randomNumber(0, verticesNum - 1);
	} while (entryVertexIndex == endVertexIndex);

	//std::cout << "Entry: " << entryVertexIndex << std::endl;
	//std::cout << "End: " << endVertexIndex << std::endl;

	graph.vertices[entryVertexIndex].attributes.isEntry = true;
	graph.vertices[endVertexIndex].attributes.isEndRoom = true;

	graph.attributes.endIndex = endVertexIndex;
	graph.attributes.entryIndex = entryVertexIndex;

	assert(integrityCheck(graph));

	return graph;
}

DLLExport Graph graph_generateStartGraph(int verticesNum, GeneticAlgorithm & ga)
{
	assert(verticesNum >= 1);

	Graph graph;

	PopId start = ga.requestId();
	PopId end = ga.requestId();

	PopId currentVertex = ga.requestId();

	graph.addEdge(start, currentVertex, false);

	for (int i = 0; i < verticesNum - 3; i++)
	{
		PopId newVertex = ga.requestId();
		graph.addEdge(currentVertex, newVertex, false);
		currentVertex = newVertex;
	}

	graph.addEdge(end, currentVertex, false);
	bool result;
	int endIndex = graph.findVertexIndexInt(end, result);
	int startIndex = graph.findVertexIndexInt(start, result);
	graph.vertices[endIndex].attributes.isEndRoom = true;
	graph.vertices[startIndex].attributes.isEntry = true;
	graph.attributes.entryIndex = startIndex;
	graph.attributes.endIndex = endIndex;

	assert(result == true);

	return graph;
}

DLLExport void graph_removeEdgeMutation(Graph& graph, Vertex& vertex)
{
	if (vertex.neighbours.size() > 1)
	{
		int neighbourIndex = randomNumber(0, vertex.neighbours.size() - 1);
		int edgeToRemoveIndex;
		int i = 0;
		for (auto n : vertex.neighbours)
		{
			if (i == neighbourIndex)
			{
				edgeToRemoveIndex = n;
				break;
			}

			i++;
		}

		if (graph.vertices[edgeToRemoveIndex].neighbours.size() > 1)
		{
			Graph dummyGraph = graph;			
			dummyGraph.removeEdgeByName(vertex.vertexID, graph.vertices[edgeToRemoveIndex].vertexID);
			bool foundIndex;
			bool isConnected = dummyGraph.IsNodeConnected(dummyGraph.findVertexIndexInt(vertex.vertexID, foundIndex)) && dummyGraph.IsNodeConnected(edgeToRemoveIndex);
			if (isConnected)
			{
				graph.removeEdgeByName(vertex.vertexID, graph.vertices[edgeToRemoveIndex].vertexID);
			}
		}

		//std::cout << "Remove edge " << graph.vertices[edgeToRemoveIndex].vertexName << " from " << vertex.vertexName << std::endl;
	}
}

DLLExport void graph_addEdgeMutation(Graph& graph, Vertex& vertex)
{
	int newEdge;

	do
	{
		newEdge = graph.vertices[randomNumber(0, graph.vertices.size() - 1)].vertexID;
	} while (newEdge == vertex.vertexID);

	graph.addEdge(vertex.vertexID, newEdge, false);

	//std::cout << "Add edge " << newEdge << " from " << vertex.vertexName << std::endl;
}

DLLExport void graph_addVertexMutation(Graph& graph, Vertex& vertex, GeneticAlgorithm& ga)
{
	Vertex newVertex(ga.requestId());
	graph.addEdge(vertex.vertexID, newVertex.vertexID, false);

	//std::cout << "Add new Vertex " << newVertex.vertexName << std::endl;
}

DLLExport void graph_swapEntryMutation(Graph& graph, GeneticAlgorithm& ga)
{
	graph.vertices[graph.attributes.entryIndex].attributes.isEntry = false;

	unsigned int newEntry = -1;

	do
	{
		newEntry = randomNumber(0, graph.vertices.size() - 1);
	} while (newEntry == graph.attributes.endIndex || graph.attributes.entryIndex == newEntry);

	graph.attributes.entryIndex = newEntry;
	graph.vertices[newEntry].attributes.isEntry = true;
	graph.vertices[newEntry].attributes.treasureRoom = false;
}

DLLExport void graph_swapEndMutation(Graph& graph, GeneticAlgorithm& ga)
{
	graph.vertices[graph.attributes.endIndex].attributes.isEndRoom = false;

	unsigned int newEnd = -1;

	std::vector<int> deadends;
	GetDeadends(graph, deadends);

	if (deadends.size() > 0)
	{
		for (int i = 0; i < deadends.size(); i++)
		{
			if (!graph.vertices[deadends[i]].attributes.isEndRoom && !graph.vertices[deadends[i]].attributes.isEntry)
			{
				newEnd = deadends[i];
			}
		}
	}

	while (newEnd == graph.attributes.endIndex || newEnd == graph.attributes.entryIndex || newEnd == -1)
	{
		newEnd = randomNumber(0, graph.vertices.size() - 1);
	}

	graph.attributes.endIndex = newEnd;
	graph.vertices[newEnd].attributes.isEndRoom = true;
	graph.vertices[newEnd].attributes.treasureRoom = false;
}

void graph_addTreasure(Graph & graph, GeneticAlgorithm & ga)
{
	int index;
	std::vector<int> deadends;

	GetDeadends(graph, deadends);
	if (deadends.size() > 0)
	{
		index = deadends[randomNumber(0, deadends.size() - 1)];
	}
	else
	{
		index = randomNumber(0, graph.vertices.size() - 1);

	}
	if (!graph.vertices[index].attributes.isEndRoom && !graph.vertices[index].attributes.isEntry)
	{
		graph.vertices[index].attributes.treasureRoom = true;
	}
}

void graph_removeTreasure(Graph & graph, GeneticAlgorithm & ga)
{
	graph.vertices[randomNumber(0, graph.vertices.size() - 1)].attributes.treasureRoom = false;
}

DLLExport void graph_addVertex1Production(Graph & graph, int vertexIndex , GeneticAlgorithm & ga)
{
	assert(!graph.vertices[vertexIndex].attributes.isEntry && !graph.vertices[vertexIndex].attributes.isEndRoom);

	int randomNeighborIndex = randomNumber(0, graph.vertices[vertexIndex].neighbours.size() - 1);
	int i = 0;
	int randomNeighbor;
	for (int neighbor : graph.vertices[vertexIndex].neighbours)
	{
		if (randomNeighborIndex == i)
		{
			randomNeighbor = neighbor;
		}

		i++;
	}
	graph.removeEdge(vertexIndex, randomNeighbor);

	PopId newVertex = ga.requestId();

	graph.addEdge(graph.vertices[vertexIndex].vertexID, newVertex, false);
	graph.addEdge(graph.vertices[randomNeighbor].vertexID, newVertex, false);

	bool result;
	graph.vertices[graph.findVertexIndexInt(newVertex, result)].attributes.Opponents = getOpponentList(ga.DProperties);
}

DLLExport void graph_addVertex2Production(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{
	assert(!graph.vertices[vertexIndex].attributes.isEntry && !graph.vertices[vertexIndex].attributes.isEndRoom);

	PopId newVertex = ga.requestId();
	graph.addEdge(graph.vertices[vertexIndex].vertexID, newVertex, false);

	bool result;
	graph.vertices[graph.findVertexIndexInt(newVertex, result)].attributes.Opponents = getOpponentList(ga.DProperties);
}

DLLExport void graph_removeVertexProduction(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{
	assert(!graph.vertices[vertexIndex].attributes.isEntry && !graph.vertices[vertexIndex].attributes.isEndRoom);


	if (graph.vertices[vertexIndex].neighbours.size() >= 2)
	{
		Graph copy = graph;

		int randomNeighborIndex1 = randomNumber(0, copy.vertices[vertexIndex].neighbours.size() - 1);
		int i = 0;
		int randomNeighbor1;
		for (int neighbor : copy.vertices[vertexIndex].neighbours)
		{
			if (randomNeighborIndex1 == i)
			{
				randomNeighbor1 = neighbor;
			}

			i++;
		}
		copy.removeEdge(vertexIndex, randomNeighbor1);
		int randomNeighborIndex2 = randomNumber(0, copy.vertices[vertexIndex].neighbours.size() - 1);
		i = 0;
		int randomNeighbor2;
		for (int neighbor : copy.vertices[vertexIndex].neighbours)
		{
			if (randomNeighborIndex2 == i)
			{
				randomNeighbor2 = neighbor;
			}

			i++;
		}
		copy.removeEdge(vertexIndex, randomNeighbor2);
		copy.addEdge(copy.vertices[randomNeighbor1].vertexID, copy.vertices[randomNeighbor2].vertexID, false);
		if (copy.RemoveVertex(vertexIndex))
		{
			graph = copy;
		}
	}
}

DLLExport void graph_addCycleProduction(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{
	assert(!graph.vertices[vertexIndex].attributes.isEntry && !graph.vertices[vertexIndex].attributes.isEndRoom);

	int randomNeighborIndex = randomNumber(0, graph.vertices[vertexIndex].neighbours.size() - 1);
	int i = 0;
	int randomNeighbor;
	for (int neighbor : graph.vertices[vertexIndex].neighbours)
	{
		if (randomNeighborIndex == i)
		{
			randomNeighbor = neighbor;
		}

		i++;
	}
	PopId newVertex1 = ga.requestId();
	PopId newVertex2 = ga.requestId();

	graph.removeEdge(vertexIndex, randomNeighbor);

	graph.addEdge(graph.vertices[vertexIndex].vertexID, newVertex1, false);
	graph.addEdge(graph.vertices[vertexIndex].vertexID, newVertex2, false);

	graph.addEdge(graph.vertices[randomNeighbor].vertexID, newVertex1, false);
	graph.addEdge(graph.vertices[randomNeighbor].vertexID, newVertex2, false);

	bool result;
	graph.vertices[graph.findVertexIndexInt(newVertex1, result)].attributes.Opponents = getOpponentList(ga.DProperties);
	graph.vertices[graph.findVertexIndexInt(newVertex2, result)].attributes.Opponents = getOpponentList(ga.DProperties);
}

DLLExport void graph_addTreasureProduction(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{
	graph.vertices[vertexIndex].attributes.treasureRoom = !graph.vertices[vertexIndex].attributes.treasureRoom;
}

DLLExport void graph_removeTreasureProduction(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{

}

void graph_addOpponent(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{
	OpponentInfo opponent = ga.DProperties.OpponentTypes[randomNumber(0, ga.DProperties.OpponentTypes.size() - 1)];
	if(graph.vertices[vertexIndex].attributes.Opponents.size() < 4)
		graph.vertices[vertexIndex].attributes.Opponents.push_back(opponent.Id);
}

std::list<int> getOpponentList(DungeonProperties & props)
{
	std::list<int> opponents;
	// should be limited by smallest room
	int numOpponents = randomNumber(0, 4);
	for (int i = 0; i < numOpponents; i++)
	{
		auto it = props.OpponentTypes.begin();
		std::advance(it, randomNumber(0, props.OpponentTypes.size() - 1));
		opponents.push_back(it->first);
	}

	return opponents;
}

void graph_removeOpponent(Graph & graph, int vertexIndex, GeneticAlgorithm & ga)
{
	std::list<int>& opponentList = graph.vertices[vertexIndex].attributes.Opponents;
	if (opponentList.size() > 0)
	{
		int deleteIndex = randomNumber(0, opponentList.size() - 1);
		auto it = opponentList.begin();
		std::advance(it, deleteIndex);
		opponentList.erase(it);
	}
}

// TODO mutation rate should not be magic values
DLLExport void graph_mutate(Graph& graph, GeneticAlgorithm& ga)
{
	for (int i = 0; i < graph.vertices.size(); i++)
	{
		int p = randomNumber(0, 100);
		//std::cout << "P: " << p << std::endl;
		if (p < 7)
		{
			graph_removeEdgeMutation(graph, graph.vertices[i]);
		}
		else if (p < 14)
		{
			graph_addEdgeMutation(graph, graph.vertices[i]);
		}
		else if (p < 21)
		{
			graph_addVertexMutation(graph, graph.vertices[i], ga);
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
}