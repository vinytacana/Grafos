#include "Algorithms.h"
#include "Utils.h"
#include <string.h>
#include <limits.h>

void algorithms_bfs(const Graph *g, int startNode, const char *outputFile) {
    int n = g->num_vertices;
    if (startNode < 0 || startNode >= n) return;
    bool *visited = (bool*)calloc(n, sizeof(bool));
    int *parent = (int*)malloc(n * sizeof(int));
    int *level = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) parent[i] = -1;
    
    Queue *q = queue_create(n);
    FILE *out = fopen(outputFile, "w");
    
    visited[startNode] = true; level[startNode] = 0;
    queue_enqueue(q, startNode);
    
    while (!queue_is_empty(q)) {
        int u = queue_dequeue(q);
        fprintf(out, "VERTICE: %d. PAI: %d. NIVEL: %d\n", u + 1, parent[u] + 1, level[u]);
        
        NeighborIterator it = graph_get_iterator(g, u);
        Edge e;
        while (graph_next_neighbor(&it, &e)) {
            if (!visited[e.to]) {
                visited[e.to] = true; parent[e.to] = u; level[e.to] = level[u] + 1;
                queue_enqueue(q, e.to);
            }
        }
    }
    fclose(out); queue_destroy(q); free(visited); free(parent); free(level);
}

void algorithms_dfs(const Graph *g, int startNode, const char *outputFile) {
    int n = g->num_vertices;
    if (startNode < 0 || startNode >= n) return;
    bool *visited = (bool*)calloc(n, sizeof(bool));
    int *parent = (int*)malloc(n * sizeof(int));
    int *level = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) parent[i] = -1;
    
    Stack *s = stack_create(n);
    FILE *out = fopen(outputFile, "w");
    stack_push(s, startNode);
    level[startNode] = 0;

    while (!stack_is_empty(s)) {
        int u = stack_pop(s);
        if (!visited[u]) {
            visited[u] = true;
            fprintf(out, "VERTICE: %d. PAI: %d. NIVEL: %d\n", u + 1, parent[u] + 1, level[u]);
            
            NeighborIterator it = graph_get_iterator(g, u);
            Edge e;
            // Note: C DFS stack visits neighbors in reverse of push order
            while (graph_next_neighbor(&it, &e)) {
                if (!visited[e.to]) {
                    parent[e.to] = u; level[e.to] = level[u] + 1;
                    stack_push(s, e.to);
                }
            }
        }
    }
    fclose(out); stack_destroy(s); free(visited); free(parent); free(level);
}

Component* algorithms_find_connected_components(const Graph *g, int *num_components) {
    int n = g->num_vertices;
    bool *visited = (bool*)calloc(n, sizeof(bool));
    Component *comps = NULL;
    *num_components = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            (*num_components)++;
            comps = (Component*)realloc(comps, (*num_components) * sizeof(Component));
            Component *c = &comps[*num_components - 1];
            c->members = NULL; c->size = 0;

            Queue *q = queue_create(n);
            queue_enqueue(q, i);
            visited[i] = true;
            
            // Temporary storage for BFS tree properties in this component
            int *parent_map = (int*)malloc(n * sizeof(int));
            int *level_map = (int*)malloc(n * sizeof(int));
            for(int j=0; j<n; j++) parent_map[j] = -1;
            level_map[i] = 0;

            while (!queue_is_empty(q)) {
                int u = queue_dequeue(q);
                c->size++;
                c->members = (ComponentMember*)realloc(c->members, c->size * sizeof(ComponentMember));
                c->members[c->size-1].vertex = u;
                c->members[c->size-1].parent = parent_map[u];
                c->members[c->size-1].level = level_map[u];

                NeighborIterator it = graph_get_iterator(g, u);
                Edge e;
                while (graph_next_neighbor(&it, &e)) {
                    if (!visited[e.to]) {
                        visited[e.to] = true;
                        parent_map[e.to] = u;
                        level_map[e.to] = level_map[u] + 1;
                        queue_enqueue(q, e.to);
                    }
                }
            }
            queue_destroy(q); free(parent_map); free(level_map);
        }
    }
    free(visited);
    // Sort components by size descending
    for (int i = 0; i < *num_components - 1; i++) {
        for (int j = i + 1; j < *num_components; j++) {
            if (comps[i].size < comps[j].size) {
                Component tmp = comps[i]; comps[i] = comps[j]; comps[j] = tmp;
            }
        }
    }
    return comps;
}

void algorithms_destroy_components(Component *components, int num_components) {
    for (int i = 0; i < num_components; i++) free(components[i].members);
    free(components);
}

float algorithms_get_distance(const Graph *g, int u, int v) {
    if (u == v) return 0;
    int n = g->num_vertices;
    float *dist = (float*)malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) dist[i] = FLT_MAX;
    Queue *q = queue_create(n);
    dist[u] = 0; queue_enqueue(q, u);
    while (!queue_is_empty(q)) {
        int curr = queue_dequeue(q);
        NeighborIterator it = graph_get_iterator(g, curr);
        Edge e;
        while (graph_next_neighbor(&it, &e)) {
            if (dist[e.to] == FLT_MAX) {
                dist[e.to] = dist[curr] + 1.0f;
                if (e.to == v) { float res = dist[e.to]; free(dist); queue_destroy(q); return res; }
                queue_enqueue(q, e.to);
            }
        }
    }
    free(dist); queue_destroy(q); return FLT_MAX;
}

float algorithms_get_diameter(const Graph *g) {
    int n = g->num_vertices; if (n == 0) return 0;
    float maxDist = 0;
    for (int i = 0; i < n; i++) {
        float *dist = (float*)malloc(n * sizeof(float));
        for (int j = 0; j < n; j++) dist[j] = FLT_MAX;
        Queue *q = queue_create(n);
        dist[i] = 0; queue_enqueue(q, i);
        while (!queue_is_empty(q)) {
            int u = queue_dequeue(q);
            if (dist[u] > maxDist) maxDist = dist[u];
            NeighborIterator it = graph_get_iterator(g, u); Edge e;
            while (graph_next_neighbor(&it, &e)) {
                if (dist[e.to] == FLT_MAX) { dist[e.to] = dist[u] + 1.0f; queue_enqueue(q, e.to); }
            }
        }
        free(dist); queue_destroy(q);
    }
    return maxDist;
}

float algorithms_get_approx_diameter(const Graph *g) {
    int n = g->num_vertices;
    if (n == 0) return 0;
    
    // 1st BFS to find farthest node from node 0
    float *dist1 = (float*)malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) dist1[i] = FLT_MAX;
    Queue *q1 = queue_create(n);
    dist1[0] = 0; queue_enqueue(q1, 0);
    int farthest_node = 0;
    while (!queue_is_empty(q1)) {
        int u = queue_dequeue(q1);
        if (dist1[u] > dist1[farthest_node]) farthest_node = u;
        NeighborIterator it = graph_get_iterator(g, u); Edge e;
        while (graph_next_neighbor(&it, &e)) {
            if (dist1[e.to] == FLT_MAX) { dist1[e.to] = dist1[u] + 1.0f; queue_enqueue(q1, e.to); }
        }
    }
    free(dist1); queue_destroy(q1);
    
    // 2nd BFS from farthest_node
    float *dist2 = (float*)malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) dist2[i] = FLT_MAX;
    Queue *q2 = queue_create(n);
    dist2[farthest_node] = 0; queue_enqueue(q2, farthest_node);
    int opposite_node = farthest_node;
    while (!queue_is_empty(q2)) {
        int u = queue_dequeue(q2);
        if (dist2[u] > dist2[opposite_node]) opposite_node = u;
        NeighborIterator it = graph_get_iterator(g, u); Edge e;
        while (graph_next_neighbor(&it, &e)) {
            if (dist2[e.to] == FLT_MAX) { dist2[e.to] = dist2[u] + 1.0f; queue_enqueue(q2, e.to); }
        }
    }
    float approx_diameter = dist2[opposite_node];
    free(dist2); queue_destroy(q2);
    return approx_diameter;
}

#include <time.h>
void algorithms_benchmark_100_runs(const Graph *g, int type) {
    int n = g->num_vertices;
    if (n == 0) return;
    double total_time = 0;
    srand((unsigned int)time(NULL));
    
    printf("\nExecutando 100 buscas para benchmark...\n");
    for (int i = 0; i < 100; i++) {
        int start = rand() % n;
        clock_t begin = clock();
        if (type == 1) algorithms_bfs(g, start, "/dev/null");
        else if (type == 2) algorithms_dfs(g, start, "/dev/null");
        else if (type == 3) {
            DijkstraResult res = algorithms_dijkstra_with_heap(g, start);
            algorithms_destroy_dijkstra_result(res);
        }
        else if (type == 4) {
            DijkstraResult res = algorithms_dijkstra_without_heap(g, start);
            algorithms_destroy_dijkstra_result(res);
        }
        clock_t end = clock();
        total_time += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    printf("Tempo medio (100 execuções): %.6fs\n", total_time / 100.0);
}

DijkstraResult algorithms_dijkstra_with_heap(const Graph *g, int startNode) {
    int n = g->num_vertices;
    DijkstraResult res;
    res.dist = (float*)malloc(n * sizeof(float));
    res.parent = (int*)malloc(n * sizeof(int));
    res.level = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) { res.dist[i] = FLT_MAX; res.parent[i] = -1; res.level[i] = 0; }
    
    MinHeap *h = minheap_create(n);
    res.dist[startNode] = 0;
    minheap_insert_or_update(h, startNode, 0);
    
    while (!minheap_is_empty(h)) {
        HeapNode node = minheap_extract_min(h);
        int u = node.vertex;
        NeighborIterator it = graph_get_iterator(g, u); Edge e;
        while (graph_next_neighbor(&it, &e)) {
            if (res.dist[u] + e.weight < res.dist[e.to]) {
                res.dist[e.to] = res.dist[u] + e.weight;
                res.parent[e.to] = u; res.level[e.to] = res.level[u] + 1;
                minheap_insert_or_update(h, e.to, res.dist[e.to]);
            }
        }
    }
    minheap_destroy(h); return res;
}

DijkstraResult algorithms_dijkstra_without_heap(const Graph *g, int startNode) {
    int n = g->num_vertices;
    DijkstraResult res;
    res.dist = (float*)malloc(n * sizeof(float));
    res.parent = (int*)malloc(n * sizeof(int));
    res.level = (int*)malloc(n * sizeof(int));
    bool *visited = (bool*)calloc(n, sizeof(bool));
    for (int i = 0; i < n; i++) { res.dist[i] = FLT_MAX; res.parent[i] = -1; res.level[i] = 0; }
    
    res.dist[startNode] = 0;
    for (int i = 0; i < n; i++) {
        int u = -1; float minDist = FLT_MAX;
        for (int j = 0; j < n; j++) { if (!visited[j] && res.dist[j] < minDist) { minDist = res.dist[j]; u = j; } }
        if (u == -1) break;
        visited[u] = true;
        NeighborIterator it = graph_get_iterator(g, u); Edge e;
        while (graph_next_neighbor(&it, &e)) {
            if (res.dist[u] + e.weight < res.dist[e.to]) {
                res.dist[e.to] = res.dist[u] + e.weight;
                res.parent[e.to] = u; res.level[e.to] = res.level[u] + 1;
            }
        }
    }
    free(visited); return res;
}

void algorithms_destroy_dijkstra_result(DijkstraResult res) { free(res.dist); free(res.parent); free(res.level); }

void algorithms_save_info(const Graph *g, const char *filename, float diameter) {
    FILE *out = fopen(filename, "w");
    fprintf(out, "INFORMACOES DO GRAFO:\n");
    fprintf(out, "QUANTIDADE DE VERTICES: %d\n", g->num_vertices);
    fprintf(out, "QUANTIDADE DE ARESTAS: %d\n", g->num_edges);
    fprintf(out, "GRAU MINIMO: %d\n", graph_get_min_degree(g));
    fprintf(out, "GRAU MAXIMO: %d\n", graph_get_max_degree(g));
    fprintf(out, "GRAU MEDIO: %.2f\n", graph_get_avg_degree(g));
    fprintf(out, "MEDIANA DE GRAU: %.2f\n", graph_get_median_degree(g));
    fprintf(out, "DIAMETRO DO GRAFO: %.2f\n", diameter);
    fclose(out);
}

void algorithms_save_components(const Component *components, int num_components, const char *filename) {
    FILE *out = fopen(filename, "a");
    for (int i = 0; i < num_components; i++) {
        fprintf(out, "\n--- COMPONENTE CONEXA %d. QTD DE VERTICES: %d\n", i + 1, components[i].size);
        for (int j = 0; j < components[i].size; j++) {
            fprintf(out, "    VERTICE: %d. PAI: %d. NIVEL: %d\n", components[i].members[j].vertex + 1, components[i].members[j].parent + 1, components[i].members[j].level);
        }
    }
    fclose(out);
}
