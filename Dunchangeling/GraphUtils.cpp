#include "Graph.h"
#include <iostream>
#include "GraphUtils.h"
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/ref.hpp>
#include <algorithm>
#include <assert.h>

BoostGraph GeneticAlgorithmUtils::ConvertToBoostGraph(Graph & graph)
{
	BoostGraph boostGraph;	

	for (auto i : graph.vertices)
	{
		boost::add_vertex(i.attributes, boostGraph);
	}

	for (int i = 0; i < graph.vertices.size(); i++)
	{
		for (auto j : graph.vertices[i].neighbours)
		{
			boost::add_edge(i, j, boostGraph);
		}
	}

	return boostGraph;
}

void GeneticAlgorithmUtils::GraphToDot(BoostGraph & graph)
{
	boost::write_graphviz(std::cout, graph);
}

struct vertex_output_visitor : public boost::planar_face_traversal_visitor
{
	std::vector<std::vector<int>> faces;

	void begin_face()
	{
		std::cout << "New face: ";
		faces.push_back(std::vector<int>());
	}
	void end_face()
	{
		std::cout << std::endl;
	}

	template <typename Vertexi>
	void next_vertex(Vertexi v)
	{
		std::cout << v << " ";
		faces.back().push_back(v);
	}
};

std::vector<std::vector<int>> GeneticAlgorithmUtils::GetGraphFaces(BoostGraph & graph)
{
	boost::property_map< BoostGraph, boost::edge_index_t >::type e_index = get(boost::edge_index, graph);
	boost::graph_traits< BoostGraph >::edges_size_type edge_count = 0;
	boost::graph_traits< BoostGraph >::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei)
		put(e_index, *ei, edge_count++);

	typedef std::vector< boost::graph_traits<BoostGraph>::edge_descriptor > vec_t;
	std::vector<vec_t> embedding(boost::num_vertices(graph));
	if (boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = graph,
		boost::boyer_myrvold_params::embedding =
		&embedding[0]
	))
		std::cout << "Input graph is planar" << std::endl;
	else
		std::cout << "Input graph is not planar" << std::endl;

	vertex_output_visitor vVis;
	boost::planar_face_traversal(graph, &embedding[0], vVis);

	return vVis.faces;
}

Chains GeneticAlgorithmUtils::ChainDecomposition(BoostGraph & graph)
{
	Chains chains;
	int numVertices = boost::num_vertices(graph);
	std::vector<bool> usedVertices = std::vector<bool>(boost::num_vertices(graph));
	std::vector<std::vector<int>> faces = GetGraphFaces(graph);

	for (int i = 0; i < numVertices; i++)
	{
		usedVertices[i] = false;
	}

	// remove biggest face, because its always useless
	int biggestFaceIndex = -1;
	int biggestFaceVertexNum = 0;
	for (int i = 0; i < faces.size(); i++)
	{
		if (faces[i].size() > biggestFaceVertexNum)
		{
			biggestFaceIndex = i;
			biggestFaceVertexNum = faces[i].size();
		}
	}

	faces.erase(faces.begin() + biggestFaceIndex);

	// get smallest face index
	if (faces.size() != 0)
	{
		int smallestFaceIndex = 0;
		for (int i = 0; i < faces.size(); i++)
		{
			if (faces[i].size() < faces[smallestFaceIndex].size())
			{
				smallestFaceIndex = i;
			}
		}

		Chain chain;

		for (auto i : faces[smallestFaceIndex])
		{
			usedVertices[i] = true;
			chain.push_back(i);
		}

		chains.push_back(chain);
		faces.erase(faces.begin() + smallestFaceIndex);
	}

	// process remaining cycles
	while (faces.size() != 0)
	{
		Chain chain = GetNeighbourCycle(graph, faces, usedVertices);
		if(chain.size() > 0)
			chains.push_back(chain);
	}

	while (std::any_of(usedVertices.begin(), usedVertices.end(), [](int v) { return !v; }))
	{
		Chain chain = GetNeighbourPath(graph, usedVertices);
		chains.push_back(chain);
	}

	return chains;
}

Chain GeneticAlgorithmUtils::GetNeighbourCycle(BoostGraph & graph, std::vector<std::vector<int>>& faces, std::vector<bool>& usedVertices)
{
	int smallestFaceIndex = -1;
	int smallestFaceSize = INT_MAX;

	for (int i = 0; i < faces.size(); i++)
	{
		std::vector<int>& face = faces[i];
		// check whether the face contains used vertices or is next to some
		if (std::any_of(face.begin(), face.end(), [usedVertices](int v) { return usedVertices[v]; }) ||
			std::any_of(face.begin(), face.end(), [graph, usedVertices](int v)
		{
			auto neighbours = boost::adjacent_vertices(v, graph);
			return std::any_of(neighbours.first, neighbours.second, [graph, usedVertices](int v) { return usedVertices[v]; });
		}))
		{
			int size = faces.size() - std::count_if(face.begin(), face.end(), [usedVertices](int v) { return usedVertices[v]; });

			if (size < smallestFaceSize)
			{
				smallestFaceIndex = i;
				smallestFaceSize = size;
			}
		}
	}

	assert(smallestFaceIndex != -1);

	auto smallestFace = faces[smallestFaceIndex];
	
	// remove all vertices 
	smallestFace.erase(std::remove_if(
		smallestFace.begin(), smallestFace.end(),
		[usedVertices](const int& x){ return usedVertices[x]; }
	), smallestFace.end());

	// remove duplicates
	std::set<int> containedInds;
	for (int i = 0; i < smallestFace.size(); i++)
	{
		int index = smallestFace[i];
		if (containedInds.find(index) == containedInds.end())
		{
			containedInds.insert(index);
		}
		else
		{
			smallestFace.erase(smallestFace.begin() + i);
		}
	}

	faces.erase(faces.begin() + smallestFaceIndex);
	int firstVertexIndex = -1;
	Chain chain;

	if (smallestFace.empty())
	{
		return chain;
	}

	for (int i = 0; i < smallestFace.size(); i++)
	{
		int vertexIndex = smallestFace[i];

		auto neighbours = boost::adjacent_vertices(vertexIndex, graph);

		if (std::any_of(neighbours.first, neighbours.second, [usedVertices](int v) { return usedVertices[v]; }))
		{
			firstVertexIndex = i;
			break;
		}
	}

	assert(firstVertexIndex != -1);

	for (int i = firstVertexIndex; i < smallestFace.size(); i++)
	{
		int vertexIndex = smallestFace[i];

		assert(!usedVertices[vertexIndex]);

		usedVertices[vertexIndex] = true;
		chain.push_back(vertexIndex);
	}

	return chain;
}

Chain GeneticAlgorithmUtils::GetNeighbourPath(BoostGraph & graph, std::vector<bool>& usedVertices)
{
	bool foundOne = false;
	Chain chain;
	int firstVertexIndex;

	bool usedVertExists = false;
	for (int i = 0; i < boost::num_vertices(graph); i++)
	{
		if (usedVertices[i])
		{
			usedVertExists = true;
			break;
		}
	}

	if (usedVertExists)
	{
		for (int i = 0; i < boost::num_vertices(graph); i++)
		{
			if (!usedVertices[i])
			{
				auto neighbours = boost::adjacent_vertices(i, graph);
				if (std::any_of(neighbours.first, neighbours.second, [usedVertices](int v) { return usedVertices[v]; }))
				{
					firstVertexIndex = i;
					foundOne = true;
					break;
				}
			}
		}
	}
	else
	{
		// find the one with only one neighbour
		for (int i = 0; i < boost::num_vertices(graph); i++)
		{
			auto neighbours = boost::adjacent_vertices(i, graph);
			int numNeighbours = 0;
			for (; neighbours.first != neighbours.second; neighbours.first++)
			{
				numNeighbours++;
			}

			if (numNeighbours == 1)
			{
				firstVertexIndex = i;
				foundOne = true;
				break;
			}
		}
	}

	assert(foundOne);

	chain.push_back(firstVertexIndex);
	usedVertices[firstVertexIndex] = true;

	while (true)
	{
		int lastIndex = chain.back();
		auto neighbours = boost::adjacent_vertices(lastIndex, graph);

		int nextVertexIndex = -1;
		for (; neighbours.first != neighbours.second; neighbours.first++)
		{
			if (!usedVertices[*neighbours.first])
			{
				nextVertexIndex = *neighbours.first;
			}
		}

		if (nextVertexIndex == -1)
		{
			break;
		}

		chain.push_back(nextVertexIndex);
		usedVertices[nextVertexIndex] = true;
	}

	return chain;
}

struct VertexDecompWriter
{
	VertexDecompWriter(BoostGraph& graph, Chains& chains) : Graph(graph), Chains(chains) {};
	template <class VertexI>
	void operator()(std::ostream& out, VertexI v)
	{
		int colorIndex = -1;
		for (int i = 0; i < Chains.size(); i++)
		{
			if (std::any_of(Chains[i].begin(), Chains[i].end(), [v](int j) { return v == j; }))
			{
				colorIndex = i;
				break;
			}
		}

		assert(colorIndex != -1 && colorIndex < NUMCOLORS);

		out << " [fillcolor=" << "\"" << NODECOLORLIST[colorIndex] << "\"" << ", style = filled]" << std::endl;
	};
	BoostGraph Graph;
	Chains Chains;
};

void GeneticAlgorithmUtils::GraphChainsDot(BoostGraph& graph, Chains chains)
{
	boost::write_graphviz(std::cout, graph, VertexDecompWriter(graph, chains));
}

