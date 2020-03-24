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
	int vertexName;

public:
	Vertex(int d): vertexName(d){}
	Vertex(): vertexName(-1){}

	bool operator<(const Vertex& ref) const
	{
		return (ref.vertexName < vertexName);
	}

	bool operator==(const Vertex& ref) const
	{
		return (ref.vertexName == vertexName);
	}
};

class Graph
{
private:

	void addEdgeIndices(int index1, int index2, bool directed);
public:
	std::vector<Vertex> vertices;

	Graph(std::vector<Vertex> vertices){ this->vertices = vertices;}
	Graph() {};

	DLLExport std::vector<int> getAllBrokenEdges() const;

	void splitGraph(const int vertex1, const int vertex2);
	DLLExport void splitGraph(int vertex1, int vertex2, Graph& part1, Graph& part2) ;

	DLLExport void clearBrokenEdges();

	bool breakEdge(const int vertexIndex1, const int vertexIndex2);
	DLLExport bool removeEdgeByName(const int name1, const int name2);

	DLLExport std::vector<Vertex>::iterator findVertexIndex(int val, bool &res);
	DLLExport void addEdge(int n1, int n2, bool directed);
	DLLExport void addEdge(int n1, int n2, bool n1BrokenEdge, bool n2brokenEdge, bool directed);
	DLLExport std::string printAsDot();
	DLLExport std::string printAsDotPlus();
	DLLExport bool BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[]);
	bool BreadthFirstSearch(int src, Graph& graph);
	DLLExport std::vector<int> shortestPath(int src, int dest);

	DLLExport bool empty() const;
	DLLExport void clear();
};