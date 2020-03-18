#pragma once

#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <random>

#define DLLExport __declspec(dllexport)

struct Vertex
{
public:
	std::unordered_set <int> neighbours;
	bool hasBrokenEdge = false;
	int data;

public:
	Vertex(int d): data(d){}
	Vertex(): data(-1){}

	bool operator<(const Vertex& ref) const
	{
		return (ref.data < data);
	}

	bool operator==(const Vertex& ref) const
	{
		return (ref.data == data);
	}
};

class Graph
{
private:
	std::vector<Vertex> vertices;

private:
	void addEdgeIndices(int index1, int index2, bool directed);
public:
	void splitGraph(const int vertex1, const int vertex2);
	DLLExport void splitGraph(const int vertex1, const int vertex2, Graph& part1, Graph& part2);

	bool breakEdge(const int vertexIndex1, const int vertexIndex2);
	DLLExport bool removeEdgeByName(const int name1, const int name2);

	DLLExport std::vector<Vertex>::iterator findVertexIndex(int val, bool &res);
	DLLExport void addEdge(int n1, int n2, bool directed);
	DLLExport void addEdge(int n1, int n2, bool n1BrokenEdge, bool n2brokenEdge, bool directed);
	DLLExport std::string printAsDot();
	DLLExport bool BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[]);
	bool BreadthFirstSearch(int src, Graph& graph);
	DLLExport std::vector<int> shortestPath(int src, int dest);

	DLLExport bool empty();
	DLLExport void clear();
};