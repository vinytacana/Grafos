#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"

typedef struct {
    int vertex;
    int parent;
    int level;
} ComponentMember;

typedef struct {
    ComponentMember *members;
    int size;
} Component;

typedef struct {
    float *dist;
    int *parent;
    int *level;
} DijkstraResult;

// Algorithms working with Neighbor Iterator (generic)
void algorithms_bfs(const Graph *g, int startNode, const char *outputFile);
void algorithms_dfs(const Graph *g, int startNode, const char *outputFile);
Component* algorithms_find_connected_components(const Graph *g, int *num_components);
void algorithms_destroy_components(Component *components, int num_components);

float algorithms_get_distance(const Graph *g, int u, int v);
float algorithms_get_diameter(const Graph *g);
float algorithms_get_approx_diameter(const Graph *g);

void algorithms_benchmark_100_runs(const Graph *g, int type); // type: 1-BFS, 2-DFS, 3-Dijkstra

DijkstraResult algorithms_dijkstra_with_heap(const Graph *g, int startNode);
DijkstraResult algorithms_dijkstra_without_heap(const Graph *g, int startNode);
void algorithms_destroy_dijkstra_result(DijkstraResult res);

void algorithms_save_info(const Graph *g, const char *filename, float diameter);
void algorithms_save_components(const Component *components, int num_components, const char *filename);

#endif
