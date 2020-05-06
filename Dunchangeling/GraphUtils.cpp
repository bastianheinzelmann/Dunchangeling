#include "Graph.h"
#include <iostream>
#include "GraphUtils.h"
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/ref.hpp>
#include <list>

BoostGraph GraphUtils::ConvertToBoostGraph(Graph & graph)
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

void GraphUtils::GraphToDot(BoostGraph & graph)
{
	boost::write_graphviz(std::cout, graph);
}

struct output_visitor : public boost::planar_face_traversal_visitor
{
	void begin_face() { std::cout << "New face: "; }
	void end_face() { std::cout << std::endl; }
};

struct vertex_output_visitor : public boost::planar_face_traversal_visitor
{
	std::list<std::list<int>> faces;

	void begin_face()
	{
		std::cout << "New face: ";
		faces.push_back(std::list<int>());
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

std::list<std::list<int>> GraphUtils::GetGraphFaces(BoostGraph & graph)
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