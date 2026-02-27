#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

typedef enum {
    GRAPH_MATRIX,
    GRAPH_LIST
} GraphType;

typedef struct Edge {
    int to;
    float weight;
    struct Edge *next; // Only for List representation
} Edge;

typedef struct {
    int num_vertices;
    int num_edges;
    GraphType type;
    bool has_negative_weights;
    void *data; // Points to Matrix (float**) or List (Edge**)
    char **vertex_names; // Mapping from ID to Name
} Graph;

// Neighbor Iterator to allow polymorphic traversal in algorithms
typedef struct {
    const Graph *g;
    int vertex;
    int current_index; // For Matrix
    Edge *current_edge; // For List
} NeighborIterator;

// Graph Core
Graph* graph_create(int n, GraphType type);
void graph_destroy(Graph *g);
void graph_add_edge(Graph *g, int u, int v, float w);
Graph* graph_load_from_file(const char *filename, GraphType type);

// Name Mapping
void graph_set_vertex_name(Graph *g, int v, const char *name);
int graph_get_vertex_id_by_name(const Graph *g, const char *name);
const char* graph_get_vertex_name(const Graph *g, int v);
void graph_load_names(Graph *g, const char *filename);

// Traversal API
NeighborIterator graph_get_iterator(const Graph *g, int v);
bool graph_next_neighbor(NeighborIterator *it, Edge *out_edge);

// Properties
int graph_get_degree(const Graph *g, int v);
int graph_get_min_degree(const Graph *g);
int graph_get_max_degree(const Graph *g);
float graph_get_avg_degree(const Graph *g);
float graph_get_median_degree(const Graph *g);
double graph_get_memory_usage(const Graph *g);

#endif
