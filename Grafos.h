#ifndef grafosLadj_h
#define grafosLadj_h
typedef struct No
{
    int w;
    struct No *prox;
} No;

typedef struct No No;
typedef struct graph
{
    int V, U;
    int *visitado;
    No **adj;

} graph;

typedef struct Info_Vertice
{
    int pai;
    int profundidade;
} Info_Vertice;

typedef struct{
    int size;
    int *vertices;
}Componet;
typedef struct{
    int numComponents;
    Componet *components;
    int size;
}ConnectedComponets;
int *cc;

No *newNo(int w);
graph *newGraph(int V);
void addAresta(graph *grafo, int v, int w);
void exibeGraph(graph *grafo);
void freeGraph(graph *grafo);
graph *initGraphArq(const char *cam);
int numArestasL(graph *grafo);
int numVertL(graph *grafo);
int grauVertL(graph *grafo, int *graus);
int grauMinL(graph *grafo);
int grauMaxL(graph *grafo);
int grauMedioL(graph *grafo);
void outputFile(graph *grafo, const char *namefile);
void dfs(graph *grafo, int v, Info_Vertice vert[], int profundidade);
int writeGeneTree(const char *namefile, int v, graph *G);
int bfs(graph *grafo, int v, Info_Vertice vert[]);
int writeGeneTreeBFS(const char *namefile, int v, graph *G);
int distance(graph *g, int src, int dest);
int diameter(graph *G);
void bfs_cc(graph *g, int v, int *visited, int *componet, int *size);

static ConnectedComponets allocCc();
static void ReallocC(ConnectedComponets cc,int newSize);
void AddVtoCc(Componet *C, int vertex);
void dfsRCc(graph *g, int v, int id);
int GraphCc(graph *G);
ConnectedComponets findCC(graph *G);
void freeCc(ConnectedComponets *cc);
int compVertexes(const void *a, const void *b);
void sortCc(ConnectedComponets *cc);
void showConnectedComponents(graph *g);
int UGraphConnect(graph G, int s, int t);
#endif