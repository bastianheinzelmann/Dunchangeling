#include "ProductionRules.h"
#include "GeneticAlgorithmUtils.h"

Graph ProductionRules::Crossover(Graph & parent1, Graph & parent2, GeneticAlgorithm & ga)
{
	int cutPosition = (parent1.vertices.size() + 1) / 2;
	Graph entryGraph, endGraph, fusedGraph;

	SplitGraph(parent1, cutPosition, parent1.attributes.entryIndex, entryGraph, ga);
	cutPosition = (parent2.vertices.size() + 1) / 2;
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

		if (brokenEdgeEnd > -1 && brokenEdgeEntry > -1)
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

void ProductionRules::SplitGraph(Graph graph, int cutPosition, int sourceIndex, Graph & splittedGraph, GeneticAlgorithm & ga)
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
					splittedGraph.addEdge(graph.vertices[u].vertexID, graph.vertices[v].vertexID, graph.vertices[u].attributes, graph.vertices[v].attributes, false);
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

void ProductionRules::Mutate(Graph & graph, GeneticAlgorithm & ga)
{
	int mutationRate = 20;

	// reduce size of very big graphs to save speed and improve their fitness
	if (graph.vertices.size() > ga.DProperties.NumRooms)
	{
		pAddVertex1 = 0;
		pAddVertex2 = 0;
		pRemoveVertex = 100;
		pAddCycle = 0;
		pAddTreasure = 0;
	}
	else if (graph.vertices.size() > ga.DProperties.NumRooms * 1.5f)
	{
		pAddVertex1 = 10;
		pAddVertex2 = 10;
		pRemoveVertex = 60;
	}
	else
	{
		pAddVertex1 = 15;
		pAddVertex2 = 15;
		pRemoveVertex = 50;
	}

	for (int i = 0; i < graph.vertices.size(); i++)
	{
		if (randomNumber(0, 100) < mutationRate)
		{
			if (!graph.vertices[i].attributes.isEndRoom && !graph.vertices[i].attributes.isEntry)
			{
				int p = randomNumber(0, 100);

				if (p < pAddVertex1)
				{
					graph_addVertex1Production(graph, i, ga);
				}
				else if (p < pAddVertex1 + pAddVertex2)
				{
					graph_addVertex2Production(graph, i, ga);
				}
				else if (p < pAddVertex1 + pAddVertex2 + pRemoveVertex)
				{
					graph_removeVertexProduction(graph, i, ga);
				}
				else if (p < pAddVertex1 + pAddVertex2 + pRemoveVertex + pAddCycle)
				{
					graph_addCycleProduction(graph, i, ga);

				}
				else if (p < pAddVertex1 + pAddVertex2 + pRemoveVertex + pAddCycle + pAddOpponent)
				{
					graph_addOpponent(graph, i, ga);
				}
				else if (p < pAddVertex1 + pAddVertex2 + pRemoveVertex + pAddCycle + pAddOpponent + pRemoveOpponent)
				{
					graph_removeOpponent(graph, i, ga);
				}
				else if (p < 100)
				{
					graph_addTreasureProduction(graph, i, ga);
				}
			}

			assert(integrityCheck(graph));
		}
	}

}

void ProductionRules::CalculateFitness(Graph & graph, GeneticAlgorithm & ga)
{
	float fitness;

	//-------------Critical Path-----------------------------
	std::vector<unsigned int> path;

	if (!ga.DProperties.FlankingRoutes)
		path = graph.shortestPath(graph.vertices[graph.attributes.entryIndex].vertexID, graph.vertices[graph.attributes.endIndex].vertexID);

	//-------------Flanking Routes-----------------------------
	float FlankingFitness = 0;

	if (ga.DProperties.FlankingRoutes)
	{
		auto paths = graph.GetAllPaths(graph.vertices[graph.attributes.entryIndex].vertexID, graph.vertices[graph.attributes.endIndex].vertexID);

		float bestFitness = graph.vertices.size() - 2;
		std::pair<int, int> bestPaths;

		int pathSurplus = 0;

		if (paths.size() > 2)
		{
			pathSurplus = paths.size() - ga.DProperties.NumFlankingRoutes;

			for (int i = 0; i < paths.size(); i++)
			{
				for (int k = i + 1; k < paths.size(); k++)
				{
					float currentFitness = CalculateFlankingFitness(paths[i], paths[k]);
					if (currentFitness < bestFitness)
					{
						bestFitness = currentFitness;
						bestPaths.first = i;
						bestPaths.second = k;
					}
				}
			}

			path = std::vector<unsigned int>(paths[bestPaths.first]);
			for (int i = 0; i < paths[bestPaths.second].size(); i++)
			{
				int vertex = paths[bestPaths.second][i];
				if (!std::any_of(path.begin(), path.end(), [vertex](int j) { return j == vertex; }))
				{
					path.push_back(vertex);
				}
			}
		}


		FlankingFitness = std::exp(bestFitness);
		FlankingFitness = bestFitness;
	}


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

	//----------------------------Opponents---------------------------------
	float opponentFitness = 0;
	if (!ga.DProperties.FlankingRoutes && false)
	{
		int previousDifficulty = 0;
		for (int i = 0; i < path.size(); i++)
		{
			int currentDifficulty = 0;
			if (!graph.vertices[path[i]].attributes.Opponents.empty())
			{
				for (auto opponentId : graph.vertices[i].attributes.Opponents)
				{
					// we have the id of our object and look up the difficulty rating
					currentDifficulty += ga.DProperties.OpponentTypes[opponentId].Difficulty;
				}
				if (currentDifficulty >= previousDifficulty)
				{
					previousDifficulty = currentDifficulty;
				}
				else
				{
					opponentFitness += std::abs(currentDifficulty - previousDifficulty) / 2.0f;
				}
			}
			else
			{
				opponentFitness += 0.5f;
			}
		}
	}
	else
	{

	}
	//----------------------------------------------------------------------

	int numSpecialRooms = specialRoomsInCritPath + specialRoomsNotInCritPath;

	// between 0 - 10 currently
	int outsideCritPath = graph.vertices.size() - path.size() - specialRoomsNotInCritPath;
	int specialRoomDifference = std::abs(ga.DProperties.NumSpecialRooms - (specialRoomsInCritPath + specialRoomsNotInCritPath));

	float critPathFitness = std::exp((float)outsideCritPath) - 1.0f;
	critPathFitness = outsideCritPath;
	float specialRoomDiffFitness = std::exp(specialRoomDifference * 5.0f) - 1.0f;
	specialRoomDiffFitness = specialRoomDifference;

	float deadEndFitness = std::exp((float)numBrokenDeadends) - 1.0f;
	deadEndFitness = numBrokenDeadends;

	float roomNumFitness = std::exp(std::abs(ga.DProperties.NumRooms - (int)graph.vertices.size())) - 1.0f;

	fitness = critPathFitness + specialRoomDiffFitness  + FlankingFitness + roomNumFitness + opponentFitness;

	graph.fitness = fitness;
}

float ProductionRules::CalculateFlankingFitness(std::vector<unsigned int>& path1, std::vector<unsigned int>& path2)
{
	// start and end are always duplicates
	int duplicates = -2;

	for (int i = 0; i < path1.size(); i++)
	{
		int currentVertex = path1[i];
		if (std::any_of(path2.begin(), path2.end(), [currentVertex](int j) { return currentVertex == j; }))
		{
			++duplicates;
		}
	}

	float difference = std::abs((int)path1.size() - (int)path2.size());
	return difference + duplicates;
}