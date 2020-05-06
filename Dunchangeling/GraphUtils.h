#pragma once

#include "Graph.h"
#include "Constants.h"
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <list>

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexAttributes, boost::property<boost::edge_index_t, int> > BoostGraph;
typedef boost::graph_traits<BoostGraph>::vertex_descriptor vertex_t;

namespace GraphUtils
{

	DLLExport BoostGraph ConvertToBoostGraph(Graph& graph);
	DLLExport void GraphToDot(BoostGraph& graph);
	DLLExport std::list<std::list<int>> GetGraphFaces(BoostGraph& graph);
}