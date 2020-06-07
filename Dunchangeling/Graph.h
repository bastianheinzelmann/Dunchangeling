#pragma once

#include <vector>
#include <unordered_set>
#include <string>
#include <random>
#include "Constants.h"
#include "VertexAttributes.h"


struct GraphAttributes
{
	int entryIndex = -1;
	int endIndex = -1;
};


struct Vertex
{
public:
	std::unordered_set <int> neighbours;
	int NumBrokenEdges = 0;
	PopId vertexID;

	VertexAttributes attributes;

public:
	Vertex(int d, VertexAttributes attr): vertexID(d), attributes(attr) {}
	Vertex(int d): vertexID(d){}
	Vertex(): vertexID(-1){}

	unsigned int getNameWithoutGeneration() { return vertexID & (1 << ID_BITS - 1); }

	bool operator<(const Vertex& ref) const
	{
		return (ref.vertexID < vertexID);
	}

	bool operator==(const Vertex& ref) const
	{
		return (ref.vertexID == vertexID);
	}
};

class Graph
{
private:

	void addEdgeIndices(unsigned int index1, unsigned int index2, bool directed);
	void removeVertex();

public:

	float fitness;

	GraphAttributes attributes;
	std::vector<Vertex> vertices;

	Graph(std::vector<Vertex> vertices){ this->vertices = vertices;}
	Graph() {};

	DLLExport std::vector<int> getAllBrokenEdges() const;

	void splitGraph(PopId vertex1, PopId vertex2);
	DLLExport void splitGraph(PopId vertex1, PopId vertex2, Graph& part1, Graph& part2) ;

	DLLExport void clearBrokenEdges();

	bool breakEdge(const int vertexIndex1, const int vertexIndex2);
	DLLExport bool removeEdgeByName(const PopId name1, const PopId name2);

	DLLExport std::vector<Vertex>::iterator findVertexIndex(int val, bool &res);
	DLLExport int findVertexIndexInt(int val, bool& res);
	DLLExport void addEdge(PopId n1, PopId n2, bool directed);
	DLLExport void addEdge(PopId n1, PopId n2, int n1BrokenEdges, int n2brokenEdges, bool directed);
	DLLExport void addEdge(PopId n1, PopId n2, VertexAttributes va1, VertexAttributes va2, bool directed);
	DLLExport std::string printAsDot() const;
	DLLExport std::string printAsDotPlus();
	DLLExport bool BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[]);
	bool BreadthFirstSearch(int src, Graph& graph);
	DLLExport std::vector<unsigned int> shortestPath(PopId src, PopId dest);
	DLLExport std::vector<std::vector<unsigned int>> GetAllPaths(PopId src, PopId dest);

	DLLExport bool empty() const;
	DLLExport void clear();

	DLLExport bool writeToFile(const char* file);

	DLLExport bool IsPlanar();

	//Fitness
	DLLExport void calculateFitness();
};

DLLExport bool operator<(const Graph &graph1, const Graph &graph2);
DLLExport bool operator>(const Graph &graph1, const Graph &graph2);
DLLExport std::ostream& operator<<(std::ostream& out, const Graph& graph);