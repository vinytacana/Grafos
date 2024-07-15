#ifndef Grafos_M
#define Grafos_M
typedef struct graph_M{
    int V, E;
    int **adj;
}graph_M;


static int **InitMatrix(int m, int n, int firstval);
static void FreeM_M(int **M, int m);
graph_M *crateGraph_M(int v);
void freeGraph_M(graph_M *G);
int insertAresta_M(graph_M *G, int u, int v);
int removeAresta_M(graph_M *G, int u, int v);
graph_M *initGraphArq_M(char *caminho);

#endif