#pragma once
#include <vector>
#include "Graph.h"
#include <random>

#define DLLExport __declspec(dllexport)

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

	DLLExport Graph graph_crossover(const Graph& graph1, const Graph& graph2)
	{
		// get the names of the splitted graphs broken vertices
		std::vector<int> brokenEdgesGraph1 = graph1.getAllBrokenEdges();
		std::vector<int> brokenEdgesGraph2 = graph2.getAllBrokenEdges();

		// fuse em
		Graph fusedGraph;
		fusedGraph = graph_fuseGraphs(graph1, graph2);

		// shuffle em brokenedges
		auto rng = std::default_random_engine{};
		std::shuffle(std::begin(brokenEdgesGraph2), std::end(brokenEdgesGraph2), rng);

		std::random_device dev;
		std::mt19937 rngg(dev());
		std::uniform_int_distribution<std::mt19937::result_type> graphDistribution(0, graph2.vertices.size());

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

		return fusedGraph;
	}
}

class GeneticAlgorithm
{
private:

public:

};
