#include "Graph.h"
#include <assert.h>
#include <list>
#include <iostream>
#include <random>
#include <Windows.h>
#include "Utils.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include "GeneticAlgorithmUtils.h"

void Graph::splitGraph(PopId vertex1, PopId vertex2)
{

}

void Graph::splitGraph( PopId vertex1, PopId vertex2, Graph & part1, Graph & part2)
{
	std::vector<unsigned int> pathIndices = shortestPath(vertex1, vertex2);

	while (!pathIndices.empty())
	{
		int deletePathIndex = randomNumber(0, pathIndices.size() - 2);
		breakEdge(pathIndices[deletePathIndex], pathIndices[deletePathIndex + 1]);
		std::cout << "Path index: " << deletePathIndex << std::endl;
		pathIndices = shortestPath(vertex1, vertex2);
	}

	// now the graph is split into two parts
	// could do a breadth first search and add every node to the new graph 
	BreadthFirstSearch(vertex1, part1);
	BreadthFirstSearch(vertex2, part2);

	assert(!part1.empty() && !part2.empty());
}

void Graph::clearBrokenEdges()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].NumBrokenEdges = 0;
	}
}

bool Graph::breakEdge(const int vertexIndex1, const int vertexIndex2)
{
	assert((vertexIndex1 > -1) && (vertexIndex1 < vertices.size()));
	assert((vertexIndex2 > -1) && (vertexIndex2 < vertices.size()));

	vertices[vertexIndex1].neighbours.erase(vertexIndex2);
	vertices[vertexIndex2].neighbours.erase(vertexIndex1);

	vertices[vertexIndex1].NumBrokenEdges++;
	vertices[vertexIndex2].NumBrokenEdges++;

	return true;
}

bool Graph::removeEdgeByName(const PopId name1, const PopId name2)
{
	bool result1, result2;
	int vertexIndex1 = findVertexIndex(name1, result1) - vertices.begin();
	int vertexIndex2 = findVertexIndex(name2, result2) - vertices.begin();

	if (!(result1 && result2))
		return false;

	vertices[vertexIndex1].neighbours.erase(vertexIndex2);
	vertices[vertexIndex2].neighbours.erase(vertexIndex1);
}

DLLExport void Graph::removeEdge(const int vertexIndex1, const int vertexIndex2)
{
	vertices[vertexIndex1].neighbours.erase(vertexIndex2);
	vertices[vertexIndex2].neighbours.erase(vertexIndex1);
}

std::vector<Vertex>::iterator Graph::findVertexIndex(int val, bool& res)
{
	std::vector<Vertex>::iterator it;
	Vertex v(val);
	it = std::find(vertices.begin(), vertices.end(), v);
	if (it != vertices.end())
	{
		res = true;
		return it;
	}
	else
	{
		res = false;
		return vertices.end();
	}
}

int Graph::findVertexIndexInt(int val, bool& res)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].vertexID == val)
		{
			res = true;
			return i;
		}
	}

	res = false;
	return -1;

	//std::vector<Vertex>::iterator it;
	//Vertex v(val);
	//it = std::find(vertices.begin(), vertices.end(), v);
	//if (it != vertices.end())
	//{
	//	res = true;
	//	return it - vertices.begin();
	//}
	//else
	//{
	//	res = false;
	//	return -1;
	//}
}

void Graph::addEdgeIndices(unsigned int index1, unsigned int index2, bool directed)
{
	if(directed)
		vertices[index1].neighbours.insert(index2);
	else
	{
		vertices[index1].neighbours.insert(index2);
		vertices[index2].neighbours.insert(index1);
	}
}

std::vector<int> Graph::getAllBrokenEdges() const
{
	std::vector<int> brokenEdges;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].NumBrokenEdges > 0)
		{
			brokenEdges.push_back(vertices[i].vertexID);
		}
	}

	return brokenEdges;
}

std::string Graph::printAsDot() const
{
	std::string result = "digraph {\n concentrate=true \n";

	for (int i = 0; i < vertices.size(); i++)
	{

		std::string connections = "";

		if (vertices[i].attributes.isEntry)
		{
			connections.append(std::to_string(vertices[i].vertexID));
			connections.append(" [style=filled, fillcolor=green]\n");
		}

		if (vertices[i].attributes.isEndRoom)
		{
			connections.append(std::to_string(vertices[i].vertexID));
			connections.append(" [style=filled, fillcolor=yellow]\n");
		}

		if (vertices[i].attributes.treasureRoom)
		{
			connections.append(std::to_string(vertices[i].vertexID));
			connections.append(" [style=filled, fillcolor=blue]\n");
		}

		if (vertices[i].NumBrokenEdges > 0)
		{
			connections.append(std::to_string(vertices[i].vertexID));
			connections.append(" [style=filled, fillcolor=red]\n");
		}

		for (int j : vertices[i].neighbours)
		{
			connections.append(std::to_string(vertices[i].vertexID));
			connections.append(" -> ");
			connections.append(std::to_string(vertices[j].vertexID));
			connections.append("\n");
		}

		if(i != vertices.size())
			connections.append("\n");
		result.append(connections);
	}

	result.append("}");
	return result;
}

bool Graph::BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[])
{
	std::list<int> queue;
	//std::vector<bool> visited = std::vector<bool>(vertices.size(), false);
	bool* visited = new bool[vertices.size()];

	for (int i = 0; i < vertices.size(); i++)
	{
		visited[i] = false;
		distanceList[i] = INT_MAX;
		predecessorsList[i] = -1;
	}

	bool result;
	std::vector<Vertex>::iterator index = findVertexIndex(src, result);
	int sourceIndex;
	if (result)
		sourceIndex = index - vertices.begin();
	else
	{
		std::cout << "Could not find vertex index" << std::endl;
		delete[](visited);
		return false;
	}

	visited[sourceIndex] = true;
	distanceList[sourceIndex] = 0;
	queue.push_back(sourceIndex);

	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		for (int v : vertices[u].neighbours)
		{
			if (visited[v] == false)
			{
				visited[v] = true;
				distanceList[v] = distanceList[u] + 1;
				predecessorsList[v] = u;
				queue.push_back(v);

				if (vertices[v].vertexID == dest)
					return true;
			}
		}
	}

	delete[](visited);

	return false;
}

bool Graph::BreadthFirstSearch(int src, Graph& graph)
{
	std::list<int> queue;
	bool* visited = new bool[vertices.size()];

	for (int i = 0; i < vertices.size(); i++)
	{
		visited[i] = false;
	}

	//--------------find index shit-----------------------

	bool result;
	std::vector<Vertex>::iterator index = findVertexIndex(src, result);
	int sourceIndex;
	if (result)
		sourceIndex = index - vertices.begin();
	else
	{
		delete[](visited);
		std::cout << "Could not find vertex index" << std::endl;
		return false;
	}

	//---------------------------------------------------

	visited[sourceIndex] = true;
	queue.push_back(sourceIndex);

	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		for (int v : vertices[u].neighbours)
		{
			graph.addEdge(vertices[u].vertexID, vertices[v].vertexID, vertices[u].NumBrokenEdges, vertices[v].NumBrokenEdges, false);
			if (visited[v] == false)
			{
				visited[v] = true;
				queue.push_back(v);
			}
		}
	}

	if (graph.vertices.empty())
	{
		Vertex poorVertex(src);
		poorVertex.NumBrokenEdges++;
		graph.vertices.push_back(poorVertex);
	}

	// check if new graph contains entry or end room
	int entryIndex = graph.findVertexIndexInt(this->vertices[attributes.entryIndex].vertexID, result);
	if (result)
	{
		graph.attributes.entryIndex = entryIndex;
		graph.vertices[entryIndex].attributes.isEntry = true;
	}

	int endIndex = graph.findVertexIndexInt(this->vertices[attributes.endIndex].vertexID, result);
	if (result)
	{
		graph.attributes.endIndex = endIndex;
		graph.vertices[endIndex].attributes.isEndRoom = true;
	}

	delete[](visited);

	assert(!graph.empty());

	return true;
}

std::vector<unsigned int> Graph::shortestPath(PopId src, PopId dest)
{
	int *pred = new int[vertices.size()];
	int *dist = new int[vertices.size()];

	std::vector<PopId> path;
	std::vector<unsigned int> pathIndices;

	if (!BreadthFirstSearch(src, dest, pred, dist))
	{
		//std::cout << "Source and dest are not connected!" << std::endl;
		delete[](pred);
		delete[](dist);
		return pathIndices;
	}

	bool result;
	int destIndex = findVertexIndex(dest, result) - vertices.begin();
	int crawl = destIndex;
	path.push_back(dest);
	pathIndices.push_back(destIndex);
	while (pred[crawl] != -1)
	{
		pathIndices.push_back(pred[crawl]);
		path.push_back(vertices[pred[crawl]].vertexID);
		crawl = pred[crawl];
	}

	delete[](pred);
	delete[](dist);

	return pathIndices;
}

bool Graph::IsNodeConnected(int vertexIndex)
{
	int *pred = new int[vertices.size()];
	int *dist = new int[vertices.size()];

	if (!BreadthFirstSearch(vertices[vertexIndex].vertexID, vertices[attributes.endIndex].vertexID, pred, dist))
		return false;
	if (!BreadthFirstSearch(vertices[vertexIndex].vertexID, vertices[attributes.entryIndex].vertexID, pred, dist))
		return false;

	delete[](pred);
	delete[](dist);

	return true;
}

bool Graph::RemoveVertex(int vertexIndex)
{
	std::list<int> deletedEdges;

	for (auto neighbor : vertices[vertexIndex].neighbours)
	{
		deletedEdges.push_back(neighbor);
	}

	for (auto n : deletedEdges)
	{
		removeEdge(vertexIndex, n);
	}

	vertices.erase(vertices.begin() + vertexIndex);

	for (int vertex = 0; vertex < vertices.size(); vertex++)
	{
		std::unordered_set<int> newNeighbours;

		for (const auto& neighbor : vertices[vertex].neighbours)
		{
			if (neighbor > vertexIndex)
			{
				newNeighbours.insert(neighbor - 1);
			}
			else
			{
				newNeighbours.insert(neighbor);
			}
		}

		vertices[vertex].neighbours = newNeighbours;
	}

	if (this->attributes.endIndex > vertexIndex) attributes.endIndex--;
	if (this->attributes.entryIndex > vertexIndex) attributes.entryIndex--;

	// check for connectivity
	int *pred = new int[vertices.size()];
	int *dist = new int[vertices.size()];


	for (auto i : deletedEdges)
	{
		int src = i;
		if (i > vertexIndex)
		{
			--src;
		}
		if (!BreadthFirstSearch(vertices[src].vertexID, vertices[attributes.endIndex].vertexID, pred, dist))
			return false;
		if (!BreadthFirstSearch(vertices[src].vertexID, vertices[attributes.entryIndex].vertexID, pred, dist))
			return false;
	}

	delete[](pred);
	delete[](dist);

	return true;
}

std::vector<std::vector<unsigned int>> Graph::GetAllPaths(PopId src, PopId dest)
{
	std::list<std::vector<unsigned int>> queue;
	std::vector<std::vector<unsigned int>> paths;

	bool resultSrc, resultDest;
	unsigned int sourceIndex = findVertexIndexInt(src, resultSrc);
	unsigned int destIndex = findVertexIndexInt(dest, resultDest);
	if (!resultSrc || !resultDest)
	{
		assert(false);
	}

	std::vector<unsigned int> path;
	path.push_back(sourceIndex);
	queue.push_back(path);

	while (!queue.empty())
	{
		path = queue.front();
		queue.pop_front();

		int lastVertex = path[path.size() - 1];

		if (lastVertex == destIndex)
		{
			paths.push_back(path);
		}
		else
		{
			for (auto i : vertices[lastVertex].neighbours)
			{
				if (!std::any_of(path.begin(), path.end(), [i](int v) { return v == i; }))
				{
					std::vector<unsigned int> newPath(path);
					newPath.push_back(i);
					queue.push_back(newPath);
				}
			}
		}
	}

	return paths;
}

bool Graph::empty() const
{
	if (vertices.size() > 0)
		return false;
	else
		return true;
}

void Graph::clear()
{
	vertices.clear();
}

bool Graph::writeToFile(const char* file)
{
	std::string text = this->printAsDot();

	if (!WriteAsync(file, text.c_str(), text.size()))
	{
		return false;
	}

	return true;
}

void Graph::calculateFitness()
{
	std::vector<unsigned int> path = shortestPath(vertices[attributes.entryIndex].vertexID, vertices[attributes.endIndex].vertexID);
	fitness = ((float)path.size() / (float)vertices.size()) * 10;
}

void Graph::addEdge(PopId n1, PopId n2, bool directed)
{
	bool foundVertex01 = false;
	bool foundVertex02 = false;

	int vit1 = findVertexIndexInt(n1, foundVertex01);
	int vit2 = findVertexIndexInt(n2, foundVertex02);

	int node1Index = -1;
	int	node2Index = -1;

	if (!foundVertex01)
	{
		Vertex v1(n1);
		vertices.push_back(v1);
		node1Index = vertices.size() - 1;
	}
	else
	{
		node1Index = vit1;
	}

	if (!foundVertex02)
	{
		Vertex v2(n2);
		vertices.push_back(v2);
		node2Index = vertices.size() - 1;
	}
	else
	{
		node2Index = vit2;
	}

	assert((node1Index > -1) && (node1Index < vertices.size()));
	assert((node2Index > -1) && (node2Index < vertices.size()));

	addEdgeIndices(node1Index, node2Index, directed);
}

void Graph::addEdge(PopId n1, PopId n2, int n1BrokenEdges, int n2brokenEdges, bool directed)
{
	bool foundVertex01 = false;
	bool foundVertex02 = false;

	std::vector<Vertex>::iterator vit1 = findVertexIndex(n1, foundVertex01);
	std::vector<Vertex>::iterator vit2 = findVertexIndex(n2, foundVertex02);

	int node1Index = -1;
	int	node2Index = -1;

	if (!foundVertex01)
	{
		Vertex v1(n1);
		vertices.push_back(v1);
		node1Index = vertices.size() - 1;
	}
	else
	{
		node1Index = vit1 - vertices.begin();
	}

	if (!foundVertex02)
	{
		Vertex v2(n2);
		vertices.push_back(v2);
		node2Index = vertices.size() - 1;
	}
	else
	{
		node2Index = vit2 - vertices.begin();
	}

	assert((node1Index > -1) && (node1Index < vertices.size()));
	assert((node2Index > -1) && (node2Index < vertices.size()));

	vertices[node1Index].NumBrokenEdges = n1BrokenEdges;
	vertices[node2Index].NumBrokenEdges = n2brokenEdges;

	addEdgeIndices(node1Index, node2Index, directed);
}

void Graph::addEdge(PopId n1, PopId n2, VertexAttributes va1, VertexAttributes va2, bool directed)
{
	bool foundVertex01 = false;
	bool foundVertex02 = false;

	std::vector<Vertex>::iterator vit1 = findVertexIndex(n1, foundVertex01);
	std::vector<Vertex>::iterator vit2 = findVertexIndex(n2, foundVertex02);

	int node1Index = -1;
	int	node2Index = -1;

	if (!foundVertex01)
	{
		Vertex v1(n1);
		vertices.push_back(v1);
		node1Index = vertices.size() - 1;
	}
	else
	{
		node1Index = vit1 - vertices.begin();
	}

	if (!foundVertex02)
	{
		Vertex v2(n2);
		vertices.push_back(v2);
		node2Index = vertices.size() - 1;
	}
	else
	{
		node2Index = vit2 - vertices.begin();
	}

	assert((node1Index > -1) && (node1Index < vertices.size()));
	assert((node2Index > -1) && (node2Index < vertices.size()));

	vertices[node1Index].attributes = va1;
	vertices[node2Index].attributes = va2;

	addEdgeIndices(node1Index, node2Index, directed);
}

bool Graph::IsPlanar()
{
	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>> boostGraph(this->vertices.size());

	for (auto i : vertices)
	{
		for (auto j : i.neighbours)
		{
			boost::add_edge(i.vertexID, vertices[j].vertexID, boostGraph);
		}
	}

	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_index_t, int>> K_5(5);

	return boost::boyer_myrvold_planarity_test(boostGraph);
}

DLLExport std::ostream& operator<<(std::ostream& out, const Graph& graph)
{
	out << graph.printAsDot();
	return out;
}

DLLExport bool operator<(const Graph &graph1, const Graph &graph2)
{
	return graph1.fitness < graph2.fitness;
}

DLLExport bool operator>(const Graph & graph1, const Graph & graph2)
{
	return graph1.fitness > graph2.fitness;
}