#pragma once

#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <random>
#include "Constants.h"

struct VertexAttributes
{
	bool isEntry = false;
	bool isEndRoom = false;
};

struct GraphAttributes
{
	int entryIndex = -1;
	int endIndex = -1;
};

struct Vertex
{
public:
	std::unordered_set <int> neighbours;
	bool hasBrokenEdge = false;
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
	DLLExport void addEdge(PopId n1, PopId n2, bool n1BrokenEdge, bool n2brokenEdge, bool directed);
	DLLExport std::string printAsDot() const;
	DLLExport std::string printAsDotPlus();
	DLLExport bool BreadthFirstSearch(int src, int dest, int predecessorsList[], int distanceList[]);
	bool BreadthFirstSearch(int src, Graph& graph);
	DLLExport std::vector<unsigned int> shortestPath(PopId src, PopId dest);

	DLLExport bool empty() const;
	DLLExport void clear();

	DLLExport bool generateGraphImage();
	DLLExport bool writeToFile(const char* file);

	//Fitness
	DLLExport void calculateFitness();
};

DLLExport bool operator<(const Graph &graph1, const Graph &graph2);
DLLExport bool operator>(const Graph &graph1, const Graph &graph2);
DLLExport std::ostream& operator<<(std::ostream& out, const Graph& graph);