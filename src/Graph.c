#include "Graph.h"
#include <string.h>

Graph* graph_create(int n, GraphType type) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    if (!g) return NULL;
    g->num_vertices = n;
    g->num_edges = 0;
    g->type = type;
    g->has_negative_weights = false;
    g->vertex_names = (char**)calloc(n, sizeof(char*));
    
    if (type == GRAPH_MATRIX) {
        float **matrix = (float**)malloc(n * sizeof(float*));
        for (int i = 0; i < n; i++) {
            matrix[i] = (float*)calloc(n, sizeof(float));
        }
        g->data = (void*)matrix;
    } else {
        Edge **adj = (Edge**)calloc(n, sizeof(Edge*));
        g->data = (void*)adj;
    }
    return g;
}

void graph_destroy(Graph *g) {
    if (!g) return;
    int n = g->num_vertices;
    if (g->vertex_names) {
        for (int i = 0; i < n; i++) {
            if (g->vertex_names[i]) free(g->vertex_names[i]);
        }
        free(g->vertex_names);
    }
    if (g->type == GRAPH_MATRIX) {
        float **matrix = (float**)g->data;
        for (int i = 0; i < n; i++) free(matrix[i]);
        free(matrix);
    } else {
        Edge **adj = (Edge**)g->data;
        for (int i = 0; i < n; i++) {
            Edge *curr = adj[i];
            while (curr) {
                Edge *tmp = curr;
                curr = curr->next;
                free(tmp);
            }
        }
        free(adj);
    }
    free(g);
}

void graph_add_edge(Graph *g, int u, int v, float w) {
    if (u < 0 || v < 0 || u >= g->num_vertices || v >= g->num_vertices) return;
    if (w < 0.0f) g->has_negative_weights = true;
    
    if (g->type == GRAPH_MATRIX) {
        float **matrix = (float**)g->data;
        if (matrix[u][v] == 0.0f) g->num_edges++;
        matrix[u][v] = w;
        matrix[v][u] = w;
    } else {
        Edge **adj = (Edge**)g->data;
        Edge *new_edge_u = (Edge*)malloc(sizeof(Edge));
        new_edge_u->to = v; new_edge_u->weight = w; new_edge_u->next = adj[u]; adj[u] = new_edge_u;
        Edge *new_edge_v = (Edge*)malloc(sizeof(Edge));
        new_edge_v->to = u; new_edge_v->weight = w; new_edge_v->next = adj[v]; adj[v] = new_edge_v;
        g->num_edges++;
    }
}

Graph* graph_load_from_file(const char *filename, GraphType type) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;
    int n;
    if (fscanf(f, "%d", &n) != 1) { fclose(f); return NULL; }
    Graph *g = graph_create(n, type);
    int u, v;
    float w;
    char line[256];
    fgets(line, sizeof(line), f); // finish line 1
    while (fgets(line, sizeof(line), f)) {
        int items = sscanf(line, "%d %d %f", &u, &v, &w);
        if (items < 2) continue;
        if (items == 2) w = 1.0f;
        graph_add_edge(g, u-1, v-1, w);
    }
    fclose(f);
    return g;
}

void graph_set_vertex_name(Graph *g, int v, const char *name) {
    if (v < 0 || v >= g->num_vertices) return;
    if (g->vertex_names[v]) free(g->vertex_names[v]);
    size_t len = strlen(name);
    g->vertex_names[v] = (char*)malloc(len + 1);
    if (g->vertex_names[v]) strcpy(g->vertex_names[v], name);
}

int graph_get_vertex_id_by_name(const Graph *g, const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->vertex_names[i] && strcmp(g->vertex_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

const char* graph_get_vertex_name(const Graph *g, int v) {
    if (v < 0 || v >= g->num_vertices) return NULL;
    return g->vertex_names[v];
}

void graph_load_names(Graph *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    int id;
    char name[256];
    while (fscanf(f, "%d", &id) == 1) {
        int next_char = fgetc(f); // skip space or comma
        if (next_char == EOF) break;
        if (fgets(name, sizeof(name), f)) {
            size_t len = strlen(name);
            while (len > 0 && (name[len-1] == '\n' || name[len-1] == '\r')) {
                name[len-1] = '\0';
                len--;
            }
            graph_set_vertex_name(g, id - 1, name);
        }
    }
    fclose(f);
}

NeighborIterator graph_get_iterator(const Graph *g, int v) {
    NeighborIterator it;
    it.g = g; it.vertex = v; it.current_index = 0;
    if (g->type == GRAPH_LIST) it.current_edge = ((Edge**)g->data)[v];
    else it.current_edge = NULL;
    return it;
}

bool graph_next_neighbor(NeighborIterator *it, Edge *out_edge) {
    if (it->g->type == GRAPH_MATRIX) {
        float **matrix = (float**)it->g->data;
        int n = it->g->num_vertices;
        while (it->current_index < n) {
            int i = it->current_index++;
            if (matrix[it->vertex][i] != 0.0f) {
                out_edge->to = i; out_edge->weight = matrix[it->vertex][i];
                return true;
            }
        }
    } else {
        if (it->current_edge) {
            out_edge->to = it->current_edge->to; out_edge->weight = it->current_edge->weight;
            it->current_edge = it->current_edge->next;
            return true;
        }
    }
    return false;
}

int graph_get_degree(const Graph *g, int v) {
    int degree = 0;
    NeighborIterator it = graph_get_iterator(g, v);
    Edge e;
    while (graph_next_neighbor(&it, &e)) degree++;
    return degree;
}

int graph_get_min_degree(const Graph *g) {
    int min_d = g->num_vertices > 0 ? graph_get_degree(g, 0) : 0;
    for (int i = 1; i < g->num_vertices; i++) {
        int d = graph_get_degree(g, i);
        if (d < min_d) min_d = d;
    }
    return min_d;
}

int graph_get_max_degree(const Graph *g) {
    int max_d = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        int d = graph_get_degree(g, i);
        if (d > max_d) max_d = d;
    }
    return max_d;
}

float graph_get_avg_degree(const Graph *g) {
    if (g->num_vertices == 0) return 0;
    return (float)(2 * g->num_edges) / g->num_vertices;
}

static int compare_int(const void *a, const void *b) { return (*(int*)a - *(int*)b); }
float graph_get_median_degree(const Graph *g) {
    int n = g->num_vertices;
    if (n == 0) return 0;
    int *degrees = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) degrees[i] = graph_get_degree(g, i);
    qsort(degrees, n, sizeof(int), compare_int);
    float median;
    if (n % 2 != 0) median = (float)degrees[n/2];
    else median = (float)(degrees[n/2 - 1] + degrees[n/2]) / 2.0f;
    free(degrees);
    return median;
}

double graph_get_memory_usage(const Graph *g) {
    if (!g) return 0.0;
    double bytes = sizeof(Graph);
    int n = g->num_vertices;
    if (g->vertex_names) {
        bytes += n * sizeof(char*);
        for (int i = 0; i < n; i++) {
            if (g->vertex_names[i]) bytes += strlen(g->vertex_names[i]) + 1;
        }
    }
    if (g->type == GRAPH_MATRIX) {
        bytes += n * sizeof(float*);
        bytes += (double)n * n * sizeof(float);
    } else {
        bytes += n * sizeof(Edge*);
        bytes += (double)2 * g->num_edges * sizeof(Edge);
    }
    return bytes / (1024.0 * 1024.0); // Converte para MB
}
