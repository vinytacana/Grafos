#ifndef Grafos_M
#define Grafos_M
typedef struct graph_M{
    int V, E;
    int **adj;
    int *visitado;
}graph_M;

typedef struct{
    int pai;
    int profundidade;
}Info_Vertice_M;


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
void dfs_M(graph_M *G, int v, Info_Vertice_M vert[]);
int writeGeneTree_M(const char *namefile, int v, graph_M *G);
int bfs_M(graph_M *G, int v, Info_Vertice_M vert[]);
int writeGeneTreeBfs_M(const char *namefile, int v, graph_M *G);
int distance_M(graph_M *G, int v, int u);
int eccentricity_M(graph_M  *G, int v);
int diameter_M(graph_M *G);
int Diameter_M(graph_M *G);

#endif