#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Grafos_M.h"
#include "Queue.h"

static int **InitMatrix(int m, int n, int firstval)
{
    int **M = (int **)malloc(m * sizeof(int *));
    if (M == NULL)
        exit(EXIT_FAILURE);
    int i, j;
    for (i = 0; i < m; i++)
    {
        M[i] = (int *)malloc(n * sizeof(int));
        if (M[i] == NULL)
        {
            if (i != 0)
                for (j = 0; j < i; j++)
                {
                    free(M[j]);
                    free(M);
                    exit(EXIT_FAILURE);
                }
            for (int j = 0; j < n; j++)
            {
                M[i][j] = firstval;
            }
        }
    }
    return M;
}
static void FreeM_M(int **M, int m)
{
    for (int i = 0; i < m; i++)
    {
        free(M[i]);
    }
    free(M);
}
graph_M *crateGraph_M(int v)
{
    graph_M *G = (graph_M *)malloc(sizeof(graph_M));
    if (G == NULL)
        exit(EXIT_FAILURE);
    G->V = v;
    G->E = 0;
    G->adj = InitMatrix(v, v, 0);
    G->visitado = (int *)malloc(v * sizeof(int));
    if (G->visitado == NULL)
        exit(EXIT_FAILURE);
    for (int i = 0; i < v; i++)
        G->visitado[i] = 0;
    return G;
}
void freeGraph_M(graph_M *G)
{
    FreeM_M(G->adj, G->V);
    free(G->visitado);
    free(G);
}
int insertAresta_M(graph_M *G, int u, int v)
{
    if (u < 0 || v < 0 || u >= G->V || v >= G->V)
        return 0;
    if (G->adj[u][v] == 0)
    {
        G->adj[u][v] = 1;
        G->adj[v][u] = 1;
        G->E++;
        return 1;
    }
    return 0;
}
int removeAresta_M(graph_M *G, int u, int v)
{
    if (u < 0 || v < 0 || u >= G->V || v >= G->V)
        return 0;
    if (G->adj[u][v] == 1)
    {
        G->adj[u][v] = 0;
        G->adj[v][u] = 0;
        G->E--;
        return 1;
    }
    return 0;
}

graph_M *initGraphArq_M(char *caminho)
{
    FILE *arq = fopen(caminho, "r");
    if (arq == NULL)
        return NULL;
    int V;
    int u, v;
    if (!fscanf(arq, "%d", &V))
    {
        fclose(arq);
        return NULL;
    }
    graph_M *G = crateGraph_M(V);
    if (G == NULL)
    {
        fclose(arq);
        return NULL;
    }
    while (fscanf(arq, "%d %d", &u, &v) != EOF)
    {
        insertAresta_M(G, u - 1, v - 1);
    }
    fclose(arq);
    return G;
}
void showGraph_M(graph_M *G)
{
    printf("\n---Matriz de adjacencia---\n");
    for (int i = 0; i < G->V; i++)
    {
        for (int j = 0; j < G->V; j++)
        {
            printf("%d", G->adj[i][j]);
        }
        printf("\n");
    }
}
void grauVert_M(graph_M *G, int *graus)
{
    for (int i = 0; i < G->V; i++)
    {
        graus[i] = 0;
        for (int j = 0; j < G->V; j++)
        {
            if (G->adj[i][j] == 1)
            {
                graus[i] += 1;
            }
        }
    }
}
int numVert_M(graph_M *G)
{
    return G->V;
}
int grauMin_M(graph_M *G)
{
    int graus[G->V];
    grauVert_M(G, graus);
    int min = graus[0];
    for (int i = 1; i < G->V; i++)
    {
        if (graus[i] < min)
        {
            min = graus[i];
        }
    }
    return min;
}
int grauMax_M(graph_M *G)
{
    int graus[G->V];
    grauVert_M(G, graus);
    int max = graus[0];
    for (int i = 0; i < G->V; i++)
    {
        if (graus[i] > max)
        {
            max = graus[i];
        }
    }
    return max;
}
int grauMedio_M(graph_M *G)
{
    int graus[G->V];
    grauVert_M(G, graus);
    int soma = 0;
    for (int i = 0; i < G->V; i++)
    {
        soma += graus[i];
    }
    return soma / G->V;
}
void outputFile_M(graph_M *G, const char *namefile)
{
    FILE *arq = fopen(namefile, "w");
    if (arq == NULL)
    {
        perror("Erro ao abrir arquivo de saida.. ");
        return;
    }
    fprintf(arq, "\nO grafo escolhido contem: \n");
    fprintf(arq, "\nNumero de vertices: %d\n", numVert_M(G));
    fprintf(arq, "\nNumero de arestas: %d\n", G->E);
    fprintf(arq, "\nGrau minimo do grafo: %d\n", grauMin_M(G));
    fprintf(arq, "\nGrau maximo de um grafo: %d\n", grauMax_M(G));
    fprintf(arq, "\nGrau medio de um grafo: %d\n", grauMedio_M(G));
    fclose(arq);
    printf("\nArquivo escrito com sucesso");
}
void dfs_M(graph_M *G, int v, Info_Vertice_M vert[])
{
    G->visitado[v] = 1;
    for (int w = 0; w < G->V; w++)
    {
        if (G->adj[v][w] == 1 && !G->visitado[w])
        {
            vert[w].pai = v;
            vert[w].profundidade = vert[v].profundidade + 1;
            dfs_M(G, w, vert);
        }
    }
}
int writeGeneTree_M(const char *namefile, int v, graph_M *G)
{
    Info_Vertice_M *vert = (Info_Vertice_M *)malloc(sizeof(Info_Vertice_M)* G->V);
    if (vert == NULL)
        exit(EXIT_FAILURE);
    for (int w = 0; w < G->V; w++)
    {
        G->visitado[w] = 0;
        vert[w].profundidade = 0;
        vert[w].pai = -1;
    }
    dfs_M(G, v, vert);
    FILE *arq = fopen(namefile, "w");
    if (!arq)
    {
        free(vert);
        perror("Erro ao abrir o arquivo");
        return 0;
    }
    for (int w = 0; w < G->V; w++)
    {
        if (G->visitado[w])
        {
            fprintf(arq, "\nVertice= %u\tPai=%d\tProfundidade=%d\t", w + 1, vert[w].pai + 1, vert[w].profundidade);
        }
    }
    fclose(arq);
    free(vert);
    return 1;
}
int bfs_M(graph_M *G, int v, Info_Vertice_M vert[]){
    queue *q= createQueue(G->V);
    G->visitado[v]=1;
    vert[v].profundidade=0;//erro de segmentacao fault aqui
    int lastVertice= v;
    enqueue(q,v);
    while(!isEmpty(q)){
        int u= dequeue(q);
        for(int w=0;w<G->V;w++){                                                                                                                                                                                                                                                                                                                                                                               if(G->adj[u][w]==1 && !G->visitado[w]){
                G->visitado[w]=1;
                vert[w].pai=u;
                vert[w].profundidade= vert[u].profundidade+1;
                enqueue(q,w);
                lastVertice= w;
            }
        }
    }
    free(q->dados);
    free(q);
    return lastVertice;
}
int writeGeneTreeBfs_M(const char *namefile, int v, graph_M *G){
    Info_Vertice_M *vert= (Info_Vertice_M *)malloc(sizeof(Info_Vertice_M)* G->V);
    if(vert==NULL)
        exit(EXIT_FAILURE);
    
    for(int w=0; w<G->V;w++){
        G->visitado[w]=0;
        vert[w].profundidade=0;
        vert[w].pai=-1;
    }
    vert[v].profundidade=0;
    bfs_M(G, v, vert);

    FILE *arq= fopen(namefile, "w");
    if(!arq){
        free(vert);
        perror("\nErro ao abrir arquivo de saida");
        return 0;
    }
    for(int w=0;w<G->V;w++){
        if(G->visitado[w]){
            fprintf(arq,"\nVertice=%u\tPai=%d\tProfundidade=%d", w+1, vert[w].pai+1, vert[w].profundidade);
        }
    }
    fclose(arq);
    free(vert);
    return 1;
}
int distance_M(graph_M *G, int v, int u){
    if(v==u)
        return 0;
    Info_Vertice_M *vert= (Info_Vertice_M *)malloc(sizeof(Info_Vertice_M )* G->V);
    if(vert==NULL)
        exit(EXIT_FAILURE);
    for(int i=0;i<G->V;i++)
        G->visitado[i]=0;
    bfs_M(G,u, vert);
    int distance= vert[u].profundidade;
    free(vert);
    return distance;
}
int eccentricity_M(graph_M  *G, int v){
    Info_Vertice_M *vert= (Info_Vertice_M *)malloc(sizeof(Info_Vertice_M)* G->V);
    if(vert==NULL)
        exit(EXIT_FAILURE);
    for(int i=0;i<G->V;i++)
        G->visitado[i]=0;
    int vertDistance= bfs_M(G, v, vert);
    int eccen= vert[vertDistance].profundidade;
    free(vert);
    return eccen;

}
int diameter_M(graph_M *G){
    int diametro =0;
    for(int i=0;i<G->V;i++){
        int dist= eccentricity_M(G, i);
        if(dist>diametro){
            diametro = dist;
        }
    }
    return diametro;
}
int Diameter_M(graph_M *G){
    Info_Vertice_M *vert=(Info_Vertice_M *)malloc(sizeof(Info_Vertice_M)* G->V);
    if(vert==NULL)
        exit(EXIT_FAILURE);
    for( int i=0;i<G->V;i++)
        G->visitado[i]=0;
    int v= bfs_M(G, 0, vert);
    for(int i=0;i<G->V;i++)
        G->visitado[i]=0;
    int u= bfs_M(G,v, vert);
    int diametro_prox= vert[u].profundidade;
    free(vert);
    return diametro_prox;    
}