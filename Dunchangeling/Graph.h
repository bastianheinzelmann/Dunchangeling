#pragma once

#include <vector>
#include <set>
#include <string>

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
	DLLExport std::vector<Vertex>::iterator findVertexIndex(int val, bool &res);
	DLLExport void addEdge(int n1, int n2, bool directed);
	DLLExport std::string printAsDot();
	DLLExport bool BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[]);
	DLLExport void shortestPath(int src, int dest);
};