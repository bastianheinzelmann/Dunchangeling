#pragma once

#include <vector>
#include <set>
#include <string>
#include <random>

#define DLLExport __declspec(dllexport)

struct Vertex
{
private:
public:
	int data;

	std::set <int> neighbours;

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
	
	void addEdgeIndices(int index1, int index2, bool directed);

public:
	void splitGraph(const int vertex1, const int vertex2);
	DLLExport void splitGraph(const int vertex1, const int vertex2, Graph& part1, Graph& part2, std::mt19937* rng);

	bool removeEdge(const int vertexIndex1, const int vertexIndex2);
	DLLExport bool removeEdgeByName(const int name1, const int name2);

	DLLExport std::vector<Vertex>::iterator findVertexIndex(int val, bool &res);
	DLLExport void addEdge(int n1, int n2, bool directed);
	DLLExport std::string printAsDot();
	DLLExport bool BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[]);
	DLLExport std::vector<int> shortestPath(int src, int dest);
};