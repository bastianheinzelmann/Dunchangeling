#pragma once
#include <vector>
#include "Graph.h"
#include "RNG.h"
#include <random>
#include <iostream>
#include <assert.h>
#include "Constants.h"
#include <chrono>

class GeneticAlgorithm
{
public:
	DLLExport PopId requestId();
	GeneticAlgorithm() {};
	DLLExport GeneticAlgorithm(unsigned int popSize, unsigned int maxGens);

	std::vector<Graph>* CurrentPopBuffer;

	std::vector<Graph> PopBuffer1;
	std::vector<Graph> PopBuffer2;

	DLLExport void generateInitialPopulation(unsigned int verticesNum, unsigned int edgesNum, unsigned int edgesTolerance);
	DLLExport void currentGenerationToFile(const char* directory);

	DLLExport void nextGeneration();
	DLLExport void run();

private:
	
	unsigned int populationSize = 0;
	unsigned int maxGenerations;
	unsigned int elitismRate = 10;
	unsigned int crossoverRate = 90;

	std::vector<Graph> BrokenPopulation;

	const unsigned int generationBits = GENERATION_BITS;
	PopId currentPopId = 0;
	unsigned int currentGeneration = 0;
};

namespace GraphUtils
{
	DLLExport int randomNumber(int min, int max)
	{
		static std::default_random_engine re{};
		re.seed(std::chrono::system_clock::now().time_since_epoch().count());
		using Dist = std::uniform_int_distribution<int>;
		static Dist uid{};
		return uid(re, Dist::param_type{ min, max });
	}

	DLLExport Graph graph_fuseGraphs(const Graph& graph1, const Graph& graph2)
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

		return Graph(vertices);
	}

	DLLExport Graph graph_mate(Graph& graph1, Graph& graph2)
	{
		assert(!graph1.empty() && !graph2.empty(), "One graph is empty during crossover");

		std::random_device dev;
		std::mt19937 rngg(dev());
		std::uniform_int_distribution<std::mt19937::result_type> graphDistribution;


		// get the names of the splitted graph's broken vertices
		std::vector<int> brokenEdgesGraph1 = graph1.getAllBrokenEdges();
		std::vector<int> brokenEdgesGraph2 = graph2.getAllBrokenEdges();

		// fuse em
		Graph fusedGraph;
		fusedGraph = graph_fuseGraphs(graph1, graph2);

		// shuffle em brokenedges
		auto rng = std::default_random_engine{};
		std::shuffle(std::begin(brokenEdgesGraph2), std::end(brokenEdgesGraph2), rng);


		graphDistribution = std::uniform_int_distribution<std::mt19937::result_type>(0, graph2.vertices.size() - 1);

		// last broken edge from graph1 gets last broken edge from graph2 and so on
		
		while (!brokenEdgesGraph1.empty())
		{
			int lastVertex1 = brokenEdgesGraph1.back();
			brokenEdgesGraph1.pop_back();

			int lastVertex2;

			if (!brokenEdgesGraph2.empty())
			{
				lastVertex2 = brokenEdgesGraph2.back();
				brokenEdgesGraph2.pop_back();
			}
			else
			{
				// select random vertex in graph2
				// this is a serious problem not really because we have graph2 i am kinda silly today and great!
				int randIndex = graphDistribution(rngg);
				lastVertex2 = graph2.vertices[randIndex].vertexName;
			}

			fusedGraph.addEdge(lastVertex1, lastVertex2, false);
		}

		fusedGraph.clearBrokenEdges();

		return fusedGraph;
	}

	DLLExport void graph_splitGraph( Graph& graph, Graph& graph1, Graph& graph2)
	{
		RNG::getInstance().setDistribution(0, graph.vertices.size() - 1);

		int randIndex = RNG::getInstance()();

		//TODO get a random neighbour (not yet)
		int randNeighbour = *graph.vertices[randIndex].neighbours.begin();

		//std::cout << "Notice me senpai!!! RandIndex: " << graph.vertices[randIndex].vertexName << " RandNeighbour " << graph.vertices[randNeighbour].vertexName << std::endl;

		graph.splitGraph(graph.vertices[randIndex].vertexName, graph.vertices[randNeighbour].vertexName, graph1, graph2);

		assert(!graph1.empty() && !graph2.empty(), "One graph is empty after split");
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

		do
		{
			endIndex = randomNumber(0, graph.vertices.size() - 1);
		} while (graph.attributes.endIndex == endIndex);

		graph.attributes.endIndex = endIndex;
	}

	DLLExport Graph graph_crossover(Graph& parent1, Graph& parent2)
	{
		Graph parent1part1, parent1part2, parent2part1, parent2part2;

		graph_splitGraph(parent1, parent1part1, parent1part2);
		graph_splitGraph(parent2, parent2part1, parent2part2);

		unsigned int partKind1 = 0, partKind2 = 0;
		if (parent1part1.attributes.entryIndex > -1) partKind1 = partKind1 | 1;
		if (parent1part1.attributes.endIndex > -1) partKind1 = partKind1 | 2;
		if (parent1part2.attributes.entryIndex > -1) partKind1 = partKind1 | 4;
		if (parent1part2.attributes.endIndex > -1) partKind1 = partKind1 | 8;

		if (parent2part1.attributes.entryIndex > -1) partKind2 = partKind2 | 1;
		if (parent2part1.attributes.endIndex > -1) partKind2 = partKind2 | 2;
		if (parent2part2.attributes.entryIndex > -1) partKind2 = partKind2 | 4;
		if (parent2part2.attributes.endIndex > -1) partKind2 = partKind2 | 8;

		Graph graph;

		int partKind = partKind1 | partKind2;

		assert(partKind != 0);

		switch (partKind)
		{
		case 0b1111: {
			graph = graph_mate(parent1part1, parent2part1);
			break;
		}
		case 0b1001: {
			graph = graph_mate(parent1part2, parent2part1);
			break;
		}
		case 0b0110: {
			graph = graph_mate(parent1part2, parent2part1);
			break;
		}
		case 0b1110: {
			if (partKind1 == 0b1100)
			{
				// need to add random end room to parent1part1
				graph_addRandomEndRoom(parent1part1);
				graph = graph_mate(parent1part1, parent2part2);
			}
			else
			{
				// add random entry to parent2part1
				graph_addRandomEntry(parent2part1);
				graph = graph_mate(parent1part1, parent2part1);
			}
			break;
		}
		case 0b1101: {
			if (partKind1 == 0b1100)
			{
				// need to add end to parent1part1
				graph_addRandomEndRoom(parent1part1);
				graph_mate(parent1part1, parent2part1);
			}
			else
			{
				// add end roim to parent part 1
				graph_addRandomEndRoom(parent2part1);
				graph_mate(parent1part1, parent2part1);
			}
			break;
		}
		case 0b0111: {
			if (partKind1 == 0b0011)
			{
				// parent1part2 needs endroom bruh
				graph_addRandomEndRoom(parent1part2);
				graph_mate(parent1part2, parent2part1);
			}
			else
			{
				// add endroo, to parent2part2
				graph_addRandomEndRoom(parent2part2);
				graph_mate(parent1part2, parent2part2);
			}
			break;
		}
		case 0b1011: {
			if (partKind1 == 0b0011)
			{
				// add entry to parent1part1
				graph_addRandomEntry(parent1part1);
				graph_mate(parent1part1, parent2part2);
			}
			else
			{
				// parent2part2 needs end room
				graph_addRandomEndRoom(parent2part2);
				graph_mate(parent1part2, parent2part2);
			}
			break;
		}
		}

		return graph_mate(parent1part1, parent1part2);
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

			graph.addEdge(graph.vertices[randVertexIndex1].vertexName, graph.vertices[randVertexIndex2].vertexName, false);
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

		// declare an entry vertex and an end vertex
		int entryVertexIndex = randomNumber(0, verticesNum - 1);
		int endVertexIndex;
		do
		{
			endVertexIndex = randomNumber(0, verticesNum - 1);
		} while (entryVertexIndex == endVertexIndex);

		std::cout << "Entry: " << entryVertexIndex << std::endl;
		std::cout << "End: " << endVertexIndex << std::endl;

		graph.vertices[entryVertexIndex].attributes.isEntry = true;
		graph.vertices[endVertexIndex].attributes.isEndRoom = true;

		graph.attributes.endIndex = endVertexIndex;
		graph.attributes.entryIndex = entryVertexIndex;

		return graph;
	}

	DLLExport Graph graph_generateRandomGraph(int verticesNum, GeneticAlgorithm& ga)
	{
		assert(verticesNum >= 2);

		Graph graph;
		int v1(ga.requestId());
		int v2(ga.requestId());
		graph.addEdge(v1, v2, false);

		for (int i = 0; i < verticesNum - 2; i++)
		{
			int newVertexName = ga.requestId();

			auto rng = std::default_random_engine{};
			std::vector<Vertex> setOfVertices = graph.vertices;
			std::shuffle(std::begin(setOfVertices), std::end(setOfVertices), rng);

			int edges = randomNumber(1, (setOfVertices.size() * 0.2) + 1);
			std::cout << "Edges " << edges << " Edges2 " << edges << std::endl;

			if (edges > setOfVertices.size())
			{
				edges = setOfVertices.size();
			}

			for (int j = 0; j < edges; j++)
			{
				int vNameOther = setOfVertices.back().vertexName;
				setOfVertices.pop_back();
				graph.addEdge(vNameOther, newVertexName, false);
			}
		}

		return graph;
	}

	DLLExport void graph_removeEdgeMutation(Graph& graph, Vertex& vertex)
	{
		if (vertex.neighbours.size() > 1)
		{
			RNG::getInstance().setDistribution(0, vertex.neighbours.size() - 1);
			int neighbourIndex = RNG::getInstance()();
			int edgeToRemoveIndex;
			int i = 0;
			for (auto n : vertex.neighbours)
			{
				if (i == neighbourIndex)
					edgeToRemoveIndex = n;

				i++;
			}

			graph.removeEdgeByName(vertex.vertexName, graph.vertices[edgeToRemoveIndex].vertexName);

			std::cout << "Remove edge " << graph.vertices[edgeToRemoveIndex].vertexName << " from " << vertex.vertexName << std::endl;
		}
	}

	DLLExport void graph_addEdgeMutation(Graph& graph, Vertex& vertex)
	{
		RNG::getInstance().setDistribution(0, graph.vertices.size() - 1);
		int newEdge;

		do
		{
			newEdge = graph.vertices[RNG::getInstance()()].vertexName;
		} while (newEdge == vertex.vertexName);

		graph.addEdge(vertex.vertexName, newEdge, false);

		std::cout << "Add edge " << newEdge << " from " << vertex.vertexName << std::endl;
	}

	DLLExport void graph_addVertexMutation(Graph& graph, Vertex& vertex, GeneticAlgorithm& ga)
	{
		Vertex newVertex(ga.requestId());
		graph.addEdge(vertex.vertexName, newVertex.vertexName, false);

		std::cout << "Add new Vertex " << newVertex.vertexName << std::endl;
	}

	DLLExport void graph_swapEntryMutation(Graph& graph, GeneticAlgorithm& ga)
	{

	}

	// TODO mutation rate should not be magic values
	DLLExport void graph_mutate(Graph& graph, GeneticAlgorithm& ga)
	{
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> percentDist;
		percentDist = std::uniform_int_distribution<std::mt19937::result_type>(0, 100);

		for (int i = 0; i < graph.vertices.size(); i++)
		{
			int p = percentDist(rng);
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
		}
	}
}
