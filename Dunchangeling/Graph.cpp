#include "Graph.h"
#include <assert.h>
#include <list>
#include <iostream>
#include <random>
#include "RNG.h"
#include <Windows.h>
#include "Utils.h"

void Graph::splitGraph(PopId vertex1, PopId vertex2)
{

}

void Graph::splitGraph( PopId vertex1, PopId vertex2, Graph & part1, Graph & part2)
{
	std::random_device dev;
	std::mt19937 rngg(dev());

	std::vector<int> pathIndices = shortestPath(vertex1, vertex2);

	while (!pathIndices.empty())
	{
		std::uniform_int_distribution<std::mt19937::result_type> graphDistribution(0, pathIndices.size() - 2);
		int deletePathIndex = graphDistribution(rngg);
		breakEdge(pathIndices[deletePathIndex], pathIndices[deletePathIndex + 1]);

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
		vertices[i].hasBrokenEdge = false;
	}
}

bool Graph::breakEdge(const int vertexIndex1, const int vertexIndex2)
{
	assert((vertexIndex1 > -1) && (vertexIndex1 < vertices.size()));
	assert((vertexIndex2 > -1) && (vertexIndex2 < vertices.size()));

	vertices[vertexIndex1].neighbours.erase(vertexIndex2);
	vertices[vertexIndex2].neighbours.erase(vertexIndex1);

	vertices[vertexIndex1].hasBrokenEdge = true;
	vertices[vertexIndex2].hasBrokenEdge = true;

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
	std::vector<Vertex>::iterator it;
	Vertex v(val);
	it = std::find(vertices.begin(), vertices.end(), v);
	if (it != vertices.end())
	{
		res = true;
		return it - vertices.begin();
	}
	else
	{
		res = false;
		return -1;
	}
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

void Graph::removeVertex()
{
	// TODO
}

std::vector<int> Graph::getAllBrokenEdges() const
{
	std::vector<int> brokenEdges;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].hasBrokenEdge)
		{
			brokenEdges.push_back(vertices[i].vertexName);
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
			connections.append(std::to_string(vertices[i].vertexName));
			connections.append(" [style=filled, fillcolor=green]\n");
		}

		if (vertices[i].attributes.isEndRoom)
		{
			connections.append(std::to_string(vertices[i].vertexName));
			connections.append(" [style=filled, fillcolor=yellow]\n");
		}

		if (vertices[i].hasBrokenEdge)
		{
			connections.append(std::to_string(vertices[i].vertexName));
			connections.append(" [style=filled, fillcolor=red]\n");
		}

		for (int j : vertices[i].neighbours)
		{
			connections.append(std::to_string(vertices[i].vertexName));
			connections.append(" -> ");
			connections.append(std::to_string(vertices[j].vertexName));
			connections.append("\n");
		}

		if(i != vertices.size())
			connections.append("\n");
		result.append(connections);
	}

	result.append("}");
	return result;
}

std::string Graph::printAsDotPlus()
{
	std::list<int> queue;
	bool* visited = new bool[vertices.size()];

	for (int i = 0; i < vertices.size(); i++)
	{
		visited[i] = false;
	}

	// starting point of breadth first search
	visited[0] = true;
	queue.push_back(0);

	std::string result = "digraph {\n";

	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();

		// mark broken edges
		if (vertices[u].hasBrokenEdge)
		{
			result.append(std::to_string(vertices[u].vertexName));
			result.append(" [style=filled, fillcolor=red]\n");
		}

		for (int v : vertices[u].neighbours)
		{
			if (visited[v] == false)
			{
				visited[v] = true;
				queue.push_back(v);

				if (vertices[v].neighbours.end() != vertices[v].neighbours.find(u))
				{
					result.append(std::to_string(vertices[u].vertexName));
					result.append("->");
					result.append(std::to_string(vertices[v].vertexName));
					result.append(" [dir=both]\n");
				}
				else
				{
					// ist nicht bidrection
					result.append(std::to_string(vertices[u].vertexName));
					result.append("->");
					result.append(std::to_string(vertices[v].vertexName));
					result.append("\n");
				}
			}
		}
	}

	delete[](visited);

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

				if (vertices[v].vertexName == dest)
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
			graph.addEdge(vertices[u].vertexName, vertices[v].vertexName, vertices[u].hasBrokenEdge, vertices[v].hasBrokenEdge, false);
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
		poorVertex.hasBrokenEdge = true;
		graph.vertices.push_back(poorVertex);
	}

	// check if new graph contains entry or end room
	int entryIndex = graph.findVertexIndexInt(this->vertices[attributes.entryIndex].vertexName, result);
	if (result)
	{
		graph.attributes.entryIndex = entryIndex;
		graph.vertices[entryIndex].attributes.isEntry = true;
	}

	int endIndex = graph.findVertexIndexInt(this->vertices[attributes.endIndex].vertexName, result);
	if (result)
	{
		graph.attributes.endIndex = endIndex;
		graph.vertices[endIndex].attributes.isEndRoom = true;
	}

	delete[](visited);

	assert(!graph.empty());

	return true;
}

std::vector<int> Graph::shortestPath(PopId src, PopId dest)
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
		path.push_back(vertices[pred[crawl]].vertexName);
		crawl = pred[crawl];
	}

	std::cout << "Shortest path length is: " << dist[destIndex];

	std::cout << "\nPath is: \n";
	for (int i = path.size() - 1; i >= 0; i--)
		std::cout << path[i] << " ";

	std::cout << std::endl;

	delete[](pred);
	delete[](dist);

	return pathIndices;
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

bool Graph::generateGraphImage()
{
	LPCTSTR graphViz = "C:/Users/Bastian/Documents/MasterStuff/UnityPlugin/Dunchangeling/packages/Graphviz.2.38.0.2/dot.exe";
	std::string str = "Tpng O test.dt";
	char* commandline = new char[str.size() + 1];
	char currentDirectory[] = "C:/Users/Bastian/Documents/MasterStuff/UnityPlugin/Dunchangeling/packages/Graphviz.2.38.0.2";
	std::copy(str.begin(), str.end(), commandline);
	commandline[str.size()] = '\0';
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//PVOID OldValue = nullptr;
	//Wow64DisableWow64FsRedirection(&OldValue);
	//ShellExecute(NULL, TEXT("open"), graphViz, commandline, NULL, SW_RESTORE);

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	bool success = CreateProcess(graphViz,
		commandline,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		currentDirectory,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	delete[](commandline);

	std::cout << "Process: " << success << std::endl;

	return true;
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

void Graph::addEdge(PopId n1, PopId n2, bool directed)
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

	addEdgeIndices(node1Index, node2Index, directed);
}

void Graph::addEdge(PopId n1, PopId n2, bool n1BrokenEdge, bool n2brokenEdge, bool directed)
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

	vertices[node1Index].hasBrokenEdge = n1BrokenEdge;
	vertices[node2Index].hasBrokenEdge = n2brokenEdge;

	addEdgeIndices(node1Index, node2Index, directed);
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