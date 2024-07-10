#ifndef grafosLadj_h
#define grafosLadj_h
typedef struct No{
    int w;
    struct No *prox;
}No;

typedef struct No No;
typedef struct graph{
    int V,U;
    int* visitado;
    No ** adj;
   
}graph;

typedef struct Info_Vertice{
    int pai;
    int profundidade;
}Info_Vertice;



No* newNo(int w);
graph* newGraph(int V);
void addAresta(graph* grafo, int v, int w);
void exibeGraph(graph* grafo);
void freeGraph(graph* grafo);
graph* initGraphArq(const char *cam);
int numArestasL(graph *grafo);
int numVertL(graph *grafo);
int grauVertL(graph *grafo, int *graus);
int grauMinL(graph *grafo);
int grauMaxL(graph *grafo);
int grauMedioL(graph *grafo);
void outputFile(graph* grafo, const char *namefile);
void dfs(graph *grafo, int v, Info_Vertice vert[], int profundidade);
int writeGeneTree(const char *namefile, int v, graph *G);
#endif