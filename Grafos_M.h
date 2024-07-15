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
void showGraph_M(graph_M *G);
void grauVert_M(graph_M *G, int *graus);
int numVert_M(graph_M *G);
int grauMin_M(graph_M *G);
int grauMax_M(graph_M *G);
int grauMedio_M(graph_M *G);
void outputFile_M(graph_M *G, const char *namefile);

#endif