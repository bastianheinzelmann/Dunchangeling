#include "Graph.h"
#include <assert.h>
#include <list>
#include <iostream>
#include <random>

void Graph::splitGraph(int vertex1, int vertex2)
{

}

void Graph::splitGraph(const int vertex1, const int vertex2, Graph & part1, Graph & part2, std::mt19937* rng)
{
	std::random_device dev;
	std::mt19937 rngg(dev());

	std::vector<int> pathIndices = shortestPath(vertex1, vertex2);

	while (!pathIndices.empty())
	{
		std::uniform_int_distribution<std::mt19937::result_type> graphDistribution(0, pathIndices.size() - 2);
		int deletePathIndex = graphDistribution(rngg);
		removeEdge(pathIndices[deletePathIndex], pathIndices[deletePathIndex + 1]);

		pathIndices = shortestPath(vertex1, vertex2);
	}
}

bool Graph::removeEdge(const int vertexIndex1, const int vertexIndex2)
{
	assert((vertexIndex1 > -1) && (vertexIndex1 < vertices.size()));
	assert((vertexIndex2 > -1) && (vertexIndex2 < vertices.size()));

	vertices[vertexIndex1].neighbours.erase(vertexIndex2);
	vertices[vertexIndex2].neighbours.erase(vertexIndex1);

	return true;
}

bool Graph::removeEdgeByName(const int name1, const int name2)
{
	bool result1, result2;
	int vertexIndex1 = findVertexIndex(name1, result1) - vertices.begin();
	int vertexIndex2 = findVertexIndex(name2, result2) - vertices.begin();

	if (!(result1 && result2))
		return false;

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

void Graph::addEdgeIndices(int index1, int index2, bool directed)
{
	if(directed)
		vertices[index1].neighbours.insert(index2);
	else
	{
		vertices[index1].neighbours.insert(index2);
		vertices[index2].neighbours.insert(index1);
	}
}

std::string Graph::printAsDot()
{
	std::string result = "digraph {\n";

	for (int i = 0; i < vertices.size(); i++)
	{
		std::string connections = "";

		for (int j : vertices[i].neighbours)
		{
			connections.append(std::to_string(vertices[i].data));
			connections.append(" -> ");
			connections.append(std::to_string(vertices[j].data));
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

				if (vertices[v].data == dest)
					return true;
			}
		}
	}

	return false;
}

std::vector<int> Graph::shortestPath(int src, int dest)
{
	int *pred = new int[vertices.size()];
	int *dist = new int[vertices.size()];

	std::vector<int> path;
	std::vector<int> pathIndices;

	if (!BreadthFirstSearch(src, dest, pred, dist))
	{
		std::cout << "Source and dest are not connected!" << std::endl;
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
		path.push_back(vertices[pred[crawl]].data);
		crawl = pred[crawl];
	}

	std::cout << "Shortest path length is: " << dist[destIndex];

	std::cout << "\nPath is: \n";
	for (int i = path.size() - 1; i >= 0; i--)
		std::cout << path[i] << " ";

	std::cout << std::endl;

	return pathIndices;
}

void Graph::addEdge(int n1, int n2, bool directed)
{

	bool foundVertex01 = false;
	bool foundVertex02 = false;

	std::vector<Vertex>::iterator vit1 = findVertexIndex(n1, foundVertex01);
	std::vector<Vertex>::iterator vit2 = findVertexIndex(n2, foundVertex02);

	//if (!foundVertex01 && !foundVertex02)
	//{
	//	return;
	//}

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

	addEdgeIndices(node1Index, node2Index, directed);
}