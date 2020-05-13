#pragma once

#include <vector>
#include "Graph.h"
#include <random>
#include "Constants.h"
#include "GeneticAlgorithm.h"

DLLExport int randomNumber(int min, int max);
DLLExport int randomFloatNumber(float min, float max);
DLLExport Graph graph_fuseGraphs(const Graph& graph1, const Graph& graph2, GeneticAlgorithm & ga);
DLLExport Graph graph_mate(Graph& graph1, Graph& graph2, GeneticAlgorithm & ga);
DLLExport void graph_splitGraph(Graph& graph, Graph& graph1, Graph& graph2);
DLLExport void graph_addRandomEntry(Graph& graph);
DLLExport void graph_addRandomEndRoom(Graph& graph);
DLLExport Graph graph_crossover(Graph& parent1, Graph& parent2, GeneticAlgorithm& ga);
DLLExport void graph_addRandomEdges(Graph& graph, int edgeNum);
DLLExport Graph graph_generateRandomGraphWilson(int verticesNum, int edgesNum, GeneticAlgorithm& ga);
DLLExport Graph graph_generateRandomGraph(int verticesNum, GeneticAlgorithm& ga);
DLLExport void graph_removeEdgeMutation(Graph& graph, Vertex& vertex);
DLLExport void graph_addEdgeMutation(Graph& graph, Vertex& vertex);
DLLExport void graph_addVertexMutation(Graph& graph, Vertex& vertex, GeneticAlgorithm& ga);
DLLExport void graph_swapEntryMutation(Graph& graph, GeneticAlgorithm& ga);
DLLExport void graph_swapEndMutation(Graph& graph, GeneticAlgorithm& ga);
DLLExport void graph_mutate(Graph& graph, GeneticAlgorithm& ga);