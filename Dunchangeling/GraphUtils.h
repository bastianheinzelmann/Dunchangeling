#pragma once

#include "Graph.h"
#include "Constants.h"
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <list>

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexAttributes, boost::property<boost::edge_index_t, int> > BoostGraph;
typedef boost::graph_traits<BoostGraph>::vertex_descriptor vertex_t;

typedef std::vector<std::vector<int>> Chains;
typedef std::vector<int> Chain;

namespace GeneticAlgorithmUtils
{
	DLLExport BoostGraph ConvertToBoostGraph(Graph& graph);
	DLLExport void GraphToDot(BoostGraph& graph);
	DLLExport std::vector<std::vector<int>> GetGraphFaces(BoostGraph& graph);
	DLLExport Chains ChainDecomposition(BoostGraph& graph);
	DLLExport Chain GetNeighbourCycle(BoostGraph& graph, std::vector<std::vector<int>>& faces, std::vector<bool>& usedVertices);
	DLLExport Chain GetNeighbourPath(BoostGraph& graph, std::vector<bool>& usedVertices);
	DLLExport void GraphChainsDot(BoostGraph& graph, Chains chains);
}