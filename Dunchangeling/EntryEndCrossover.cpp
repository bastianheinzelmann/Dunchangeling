#include "EntryEndCrossover.h"
#include "GeneticAlgorithmUtils.h"

Graph EntryEndCrossover::Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga)
{
	int cutPosition = randomNumber(1, parent1.vertices.size() - 2);
	Graph entryGraph, endGraph, fusedGraph;

	SplitGraph(parent1, cutPosition, parent1.attributes.entryIndex, entryGraph, ga);
	cutPosition = parent2.vertices.size() - entryGraph.vertices.size();
	SplitGraph(parent2, cutPosition, parent2.attributes.endIndex, endGraph, ga);

	//std::cout << "Entry graph: " << entryGraph << std::endl;
	//std::cout << "End graph: " << endGraph << std::endl;

	std::vector<int> brokenEdgesEntry = entryGraph.getAllBrokenEdges();
	std::vector<int> brokenEdgesEnd = endGraph.getAllBrokenEdges();

	fusedGraph = graph_fuseGraphs(entryGraph, endGraph, ga);

	//std::cout << "Connecting broken edges\n";

	while (!brokenEdgesEnd.empty() || !brokenEdgesEntry.empty())
	{
		int brokenEdgeEnd;
		int brokenEdgeEntry;

		if (!brokenEdgesEnd.empty())
		{
			int randBrokenListIndex = randomNumber(0, brokenEdgesEnd.size() - 1);
			brokenEdgeEnd = brokenEdgesEnd[randBrokenListIndex];
			bool foundIndex;
			int index = fusedGraph.findVertexIndexInt(brokenEdgeEnd, foundIndex);
			//std::cout << "Decrease " << brokenEdgeEnd << " by one. Currently: " << fusedGraph.vertices[index].NumBrokenEdges << std::endl;
			assert(fusedGraph.vertices[index].NumBrokenEdges > 0);
			fusedGraph.vertices[index].NumBrokenEdges -= 1;
			//std::cout << "Decreased " << brokenEdgeEnd << " by one. Currently: " << fusedGraph.vertices[index].NumBrokenEdges << std::endl;
			if (fusedGraph.vertices[index].NumBrokenEdges == 0)
			{
				brokenEdgesEnd.erase(brokenEdgesEnd.begin() + randBrokenListIndex);
				//std::cout << "Delete " << brokenEdgeEnd << std::endl;
				//for (int i = 0; i < brokenEdgesEnd.size(); i++)
				//{
				//	std::cout << brokenEdgesEnd[i] << std::endl;
				//}
			}
		}
		else
		{
			if (randomNumber(0, 1) == 1)
			{
				brokenEdgeEnd = endGraph.vertices[randomNumber(0, endGraph.vertices.size() - 1)].vertexID;
			}
			else
			{
				brokenEdgeEnd = -1;
			}
		}

		if (!brokenEdgesEntry.empty())
		{
			int randBrokenListIndex = randomNumber(0, brokenEdgesEntry.size() - 1);
			brokenEdgeEntry = brokenEdgesEntry[randBrokenListIndex];
			bool foundIndex;
			int index = fusedGraph.findVertexIndexInt(brokenEdgeEntry, foundIndex);
			//std::cout << "Decrease " << brokenEdgeEntry << " by one. Currently: " << fusedGraph.vertices[index].NumBrokenEdges << std::endl;
			assert(fusedGraph.vertices[index].NumBrokenEdges > 0);
			fusedGraph.vertices[index].NumBrokenEdges -= 1;
			//std::cout << "Decreased " << brokenEdgeEntry << " by one. Currently: " << fusedGraph.vertices[index].NumBrokenEdges << std::endl;
			if (fusedGraph.vertices[index].NumBrokenEdges == 0)
			{
				brokenEdgesEntry.erase(brokenEdgesEntry.begin() + randBrokenListIndex);
				//std::cout << "Delete " << brokenEdgeEntry << std::endl;
				//for (int i = 0; i < brokenEdgesEntry.size(); i++)
				//{
				//	std::cout << brokenEdgesEntry[i] << std::endl;
				//}
			}
		}
		else
		{
			if (randomNumber(0, 1) == 1)
			{
				brokenEdgeEntry = entryGraph.vertices[randomNumber(0, entryGraph.vertices.size() - 1)].vertexID;
			}
			else
			{
				brokenEdgeEntry = -1;
			}
		}

		if(brokenEdgeEnd > -1 && brokenEdgeEntry > -1)
			fusedGraph.addEdge(brokenEdgeEntry, brokenEdgeEnd, false);
	}

	if (fusedGraph.vertices.size() < parent1.vertices.size())
	{
		fusedGraph = parent1;
		for (int i = 0; i < parent1.vertices.size(); i++)
		{
			parent1.vertices[i].vertexID = ga.requestId();
		}
	}

	assert(integrityCheck(fusedGraph));

	return fusedGraph;
}

void EntryEndCrossover::SplitGraph(Graph graph, int cutPosition, int sourceIndex, Graph & splittedGraph, GeneticAlgorithm & ga)
{
	assert(splittedGraph.vertices.size() == 0);

	// we generating a new graph so rename all vertices
	for (int i = 0; i < graph.vertices.size(); i++)
	{
		graph.vertices[i].vertexID = ga.requestId();
	}

	// breadth first search
	std::list<int> queue;
	int consumedVertices = 0;

	bool* visited = new bool[graph.vertices.size()];

	for (int i = 0; i < graph.vertices.size(); i++)
	{
		visited[i] = false;
	}

	visited[sourceIndex] = true;
	queue.push_back(sourceIndex);
	Vertex vertex;
	vertex.attributes = graph.vertices[sourceIndex].attributes;
	vertex.vertexID = graph.vertices[sourceIndex].vertexID;
	splittedGraph.vertices.push_back(vertex);
	++consumedVertices;

	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		for (int v : graph.vertices[u].neighbours)
		{
			if (!visited[v])
			{
				visited[v] = true;
				if (!graph.vertices[v].attributes.isEndRoom && !graph.vertices[v].attributes.isEntry && consumedVertices < cutPosition)
				{
					splittedGraph.addEdge(graph.vertices[u].vertexID, graph.vertices[v].vertexID,graph.vertices[u].attributes, graph.vertices[v].attributes, false);
					++consumedVertices;
					queue.push_back(v);
				}
				else
				{
					bool result;
					int index = splittedGraph.findVertexIndexInt(graph.vertices[u].vertexID, result);
					if (!result)
					{
						assert(false);
					}
					++splittedGraph.vertices[index].NumBrokenEdges;
				}
			}
		}
	}

	for (int i = 0; i < splittedGraph.vertices.size(); i++)
	{
		if (splittedGraph.vertices[i].attributes.isEndRoom)
		{
			splittedGraph.attributes.endIndex = i;
		}
		if (splittedGraph.vertices[i].attributes.isEntry)
		{
			splittedGraph.attributes.entryIndex = i;
		}
	}

	delete[](visited);
}

void EntryEndCrossover::Mutate(Graph & graph, GeneticAlgorithm & ga)
{
	int numMutations = randomNumber(1, graph.vertices.size() - 1);

	for (int i = 0; i < numMutations; i++)
	{
		int p = randomNumber(0, 100);
		int vertexIndex = randomNumber(0, graph.vertices.size() - 1);

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
	}

	assert(integrityCheck(graph));
}

void EntryEndCrossover::CalculateFitness(Graph & graph, GeneticAlgorithm & ga)
{
	float fitness;

	//-------------Critical Path-----------------------------
	std::vector<unsigned int> path = graph.shortestPath(graph.vertices[graph.attributes.entryIndex].vertexID, graph.vertices[graph.attributes.endIndex].vertexID);

	//-------------Flanking Routes-----------------------------

	auto paths = graph.GetAllPaths(graph.attributes.entryIndex, graph.attributes.endIndex);
	

	//-----------SPECIAL ROOMS STUFF---------------------------

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

	//---------------------------DEAD ENDS-----------------------------------

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

	//----------------------------------------------------------------------

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