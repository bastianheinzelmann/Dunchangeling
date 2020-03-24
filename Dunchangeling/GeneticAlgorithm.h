#pragma once
#include <vector>
#include "Graph.h"
#include "RNG.h"
#include <random>
#include <iostream>
#include <assert.h>

#define DLLExport __declspec(dllexport)

typedef unsigned int PopId;

const int GENERATION_BITS = 10;
const int ID_BITS = 32 - GENERATION_BITS;

class GeneticAlgorithm
{
public:
	DLLExport PopId requestId();

private:
	const unsigned int generationBits = GENERATION_BITS;
	PopId currentPopId = 0;
	unsigned int currentGeneration = 0;
};

namespace GraphUtils
{

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

		std::cout << "Notice me senpai!!! RandIndex: " << graph.vertices[randIndex].vertexName << " RandNeighbour " << graph.vertices[randNeighbour].vertexName << std::endl;

		graph.splitGraph(graph.vertices[randIndex].vertexName, graph.vertices[randNeighbour].vertexName, graph1, graph2);

		assert(!graph1.empty() && !graph2.empty(), "One graph is empty after split");
	}

	DLLExport void graph_crossover(Graph& parent1, Graph& parent2)
	{
		Graph parent1part1, parent1part2, parent2part1, parent2part2;

		graph_splitGraph(parent1, parent1part1, parent1part2);
		graph_splitGraph(parent2, parent2part1, parent2part2);

		graph_mate(parent1part1, parent1part2);
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

			RNG::getInstance().setDistribution(1, (setOfVertices.size() * 0.2) + 1);
			int edges = RNG::getInstance()();

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
}
